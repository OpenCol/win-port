#!/usr/bin/env python3
"""The same question as tests/differential.py, asked in 16 bits.

The host harness compares a port built by a HOST compiler. That is fast, runs in CI and
needs nothing but clang -- and it cannot represent a far pointer in four bytes, cannot
wrap an index inside a 64 KiB segment, and knows nothing about calling convention or
stack discipline. Those are not small exclusions: 271 functions touch a pointer-valued
global or struct member and are refused by tools/port.py for exactly that reason.

This asks the same question of the SAME source, compiled by the same Borland C++ 4.52
that built the game, and run inside the emulator beside the shipped function. Everything
the host cannot represent is native here.

WHAT MAKES IT POSSIBLE. Built with -DCOL_EMU, col/segments.h defines each segment as a
compile-time far constant at the SHIPPED selector, so a ported unit compiles to code with
no relocations and no external references at all -- it addresses segment 20 at
0x10a0:0000 directly. That object can be dropped into the emulator as-is.

WHAT IT CANNOT DO YET. A function that calls anything -- another ported function, or a C
runtime helper like F_SCOPY behind a struct assignment -- compiles with a fixup for that
call, and nothing here resolves fixups. Those are reported as unsupported rather than
skipped quietly.

    tests/emu_differential.py tests/cases/job_goods.json
    tests/emu_differential.py --all
"""
import argparse, glob, json, os, sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
SRC = os.environ.get('COL_SRC_REPO',
                     os.path.expanduser('~/Repositories/dderevjanik/col-win-re'))
sys.path.insert(0, os.path.join(SRC, 'tools', 're_agent'))
sys.path.insert(0, os.path.join(SRC, 'tools'))
sys.path.insert(0, os.path.join(REPO, 'tools'))
FLAGS = '-ml -WS -Ot -3 -dc -DCOL_EMU'


EXTDEF_RE = None
FIXUP_RE = None


def _res():
    global EXTDEF_RE, FIXUP_RE
    import re
    if EXTDEF_RE is None:
        # TDUMP writes the first entry on the EXTDEF line and the rest on continuation
        # lines with only the index, so keying on the word EXTDEF finds one of them.
        EXTDEF_RE = re.compile(r"^(?:\S+\s+EXTDEF\s+|\s+)(\d+)\s*:\s*'([^']+)'",
                               re.M)
        FIXUP_RE = re.compile(r"FixUp:\s*([0-9a-fA-F]+)\s+Mode:\s*(\w+)\s+"
                              r"Loc:\s*(\w+)\s+Frame:\s*\S+\s+Target:\s*EI\[(\d+)\]")
    return EXTDEF_RE, FIXUP_RE


def symbols():
    """name -> (segment, offset) for every function the shipped image has a label for.

    Borland's own runtime is linked into the shipped binary and the disassembly labels it
    in upper case -- STRCPY at 1:0efe, F_SCOPY at 1:5000 -- so a port that calls strcpy,
    or that assigns one struct to another and gets an F_SCOPY@ call for it, resolves to
    the very code the shipped function calls.
    """
    labels = json.load(open(os.path.join(SRC, 're', 'symbols.json')))['labels']
    out = {}
    for key, nm in labels.items():
        # `L14_616a` is segment 14 DECIMAL at offset 0x616a -- TextColor, in the gfx
        # segment whose selector is 0x1068. Reading the segment as hex made that 20, a
        # DATA segment, and every callee in segment 10 or above resolved to an address in
        # the wrong place.
        seg, _, off = key[1:].partition('_')
        out.setdefault(nm, (int(seg, 10), int(off, 16)))
    for nm in list(out):
        out.setdefault(nm.lower(), out[nm])
        out.setdefault(nm + '@', out[nm])
    return out


def resolve(code, td, sym, syms):
    """Cut `sym` out of the object and point every call at the SHIPPED callee.

    A ported function that calls another function compiles to a far call with a fixup.
    Pointing that call at the shipped implementation is not a compromise -- it is the
    right experiment: this harness is asking whether THIS function behaves like its
    shipped counterpart, and giving both the same callees is what isolates it. It is also
    what the byte-exact repo's own per-function differential does.

    Returns (bytes, unresolved names).
    """
    import bytematch as BM
    ext_re, fix_re = _res()
    pubs = BM.parse_pubdefs(td)
    name = next((c for c in (sym, '_' + sym) if c in pubs), None)
    if name is None:
        return None, ['%s not in PUBDEFs' % sym]
    off = pubs[name]
    later = [o for o in pubs.values() if o > off]
    end = min(later) if later else len(code)
    extdefs = {int(m.group(1)): m.group(2) for m in ext_re.finditer(td)}
    out, missing = bytearray(code[off:end]), []
    for m in fix_re.finditer(td):
        at, loc, ei = int(m.group(1), 16), m.group(3), int(m.group(4))
        if not (off <= at < end):
            continue
        target = extdefs.get(ei, '?').lstrip('_')
        if loc != 'Pointer16' or target not in syms:
            missing.append('%s (%s)' % (target, loc))
            continue
        seg, toff = syms[target]
        sel = 0x1000 + (seg - 1) * 8
        out[at - off:at - off + 4] = bytes([toff & 0xFF, toff >> 8,
                                            sel & 0xFF, sel >> 8])
    return bytes(out), missing


def seg_of(spec):
    where, _, off = spec.partition(':')
    return (34 if where == 'DGROUP' else int(where[3:])), int(off, 0)


def observed(m, addr, args, max_instr):
    import probe
    o = probe.observe(m, addr, args, max_instr)
    writes = {}
    for w in m.log['writes']:
        if w.get('space') != 'data' or not w.get('addr'):
            continue
        sel, _, off = w['addr'].partition(':')
        seg = (int(sel, 16) - 0x1000) // 8 + 1
        for k in range(w.get('size', 1)):
            writes['%d:%04x' % (seg, int(off, 16) + k)] = \
                m.read(seg, int(off, 16) + k, 1)[0]
    return {'dxax': o['return_dxax'], 'returned': o['reached_return'],
            'sp_delta': o['sp_delta'], 'writes': writes}


BUILT = {}
SYMS = {}


def prebuild(cases):
    """Compile every case's source in a few DOSBox sessions rather than one each.

    Booting DOSBox costs seconds and compiling one of these files costs a fraction of
    one, so a file-at-a-time loop spends its whole run on the emulator.
    """
    import bcc
    SYMS.update(symbols())
    srcs = sorted({os.path.join(REPO, json.load(open(c))['source']) for c in cases})
    srcs = [s for s in srcs if os.path.exists(s)]
    print('compiling %d units with Borland C++ 4.52 (-DCOL_EMU)' % len(srcs))
    BUILT.update(bcc.compile_all(srcs, FLAGS, tdump=True))


def run_case(path, verbose=False):
    import bcc, bytematch as BM, probe
    case = json.load(open(path))
    name = case['symbol']
    src = os.path.join(REPO, case['source'])
    if not os.path.exists(src):
        print('%-32s no source at %s' % (name, case['source']))
        return None

    if not SYMS:
        SYMS.update(symbols())
    if src in BUILT:
        obj, log, td = BUILT[src]
    else:
        obj, log, td = bcc.compile_unit(src, FLAGS, tdump=True)
    if obj is None:
        print('%-32s BORLAND BUILD FAILED\n%s' % (name, (log or '')[:600]))
        return False
    if case.get('near_callees'):
        print('%-32s unsupported: the shipped code reaches %s with a near call from '
              'inside its own segment, and the candidate runs in a selector of its own'
              % (name, ', '.join(case['near_callees'][:2])))
        return None
    code, _ = BM.parse_tdump(td)
    sliced, missing = resolve(code, td, name, SYMS)
    if sliced is None:
        print('%-32s %s' % (name, missing[0]))
        return False
    if missing:
        print('%-32s unsupported: cannot resolve %s'
              % (name, ', '.join(sorted(set(missing))[:3]))) 
        return None

    m, meta = probe.build(case.get('state', 'shipped'), case.get('stubs', []))
    for spec, hexbytes in case.get('stage', {}).items():
        seg, off = seg_of(spec)
        m.write(seg, off, bytes.fromhex(hexbytes))
    for at, points_to in case.get('pointers', {}).items():
        seg, off = seg_of(at)
        tseg, toff = seg_of(points_to)
        sel = 0x1000 + (tseg - 1) * 8
        m.write(seg, off, bytes([toff & 0xFF, toff >> 8, sel & 0xFF, sel >> 8]))
    pristine = {}
    for idx, seg in m.image.segs.items():
        if m.image.is_code(idx):
            continue
        n = min(seg.get('min_alloc') or seg['length'] or 0x10000, 0x10000)
        pristine[idx] = m.read(idx, 0, n)
    cand = m.load_candidate(sliced)

    argsets = probe.parse_sweep(case.get('sweep', []), case.get('call_args', []))
    # A returned far pointer is a real value here and not on a host, so the case carries
    # both widths and each harness uses its own.
    bits = case.get('ret_bits_emu', case.get('ret_bits', 16))
    mask = (1 << bits) - 1 if bits else 0
    bad = dropped = 0
    for args in argsets:
        for idx, data in pristine.items():
            m.write(idx, 0, data)
        a = observed(m, case['shipped'], args, case.get('max_instr', 200000))
        for idx, data in pristine.items():
            m.write(idx, 0, data)
        b = observed(m, cand, args, case.get('max_instr', 200000))
        if not a['returned']:
            dropped += 1
            continue
        why = []
        if mask and (a['dxax'] & mask) != (b['dxax'] & mask):
            why.append('returned 0x%x, shipped 0x%x (%d bits)'
                       % (b['dxax'] & mask, a['dxax'] & mask, bits))
        if a['sp_delta'] != b['sp_delta']:
            why.append('stack delta %d, shipped %d' % (b['sp_delta'], a['sp_delta']))
        for k in sorted(set(a['writes']) | set(b['writes'])):
            if a['writes'].get(k) != b['writes'].get(k):
                why.append('%s: shipped %s, the port %s'
                           % (k, a['writes'].get(k), b['writes'].get(k)))
        if why:
            bad += 1
            print('  DIFF args=%s' % (list(args),))
            for w in why[:6]:
                print('       ' + w)
        elif verbose:
            print('  ok   args=%s' % (list(args),))
    total = len(argsets) - dropped
    if total == 0:
        # Every set was dropped because the shipped function never returned. Zero of zero
        # agreeing is not agreement, and counting it as such would make an unrunnable
        # function look verified.
        print('%-32s UNVERIFIED: the shipped function returned from none of the %d '
              'argument sets' % (name, dropped))
        return None
    print('%-32s %d/%d argument sets agree (16-bit, state: %s%s)'
          % (name, total - bad, total, meta.get('state'),
             ', %d dropped' % dropped if dropped else ''))
    return bad == 0


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('case', nargs='?')
    ap.add_argument('--all', action='store_true')
    ap.add_argument('-v', '--verbose', action='store_true')
    a = ap.parse_args()
    cases = sorted(glob.glob(os.path.join(HERE, 'cases', '*.json'))) if a.all \
        else [a.case]
    if not cases or cases == [None]:
        ap.error('name a case or pass --all')
    if len(cases) > 1:
        prebuild(cases)
    results = [run_case(c, a.verbose) for c in cases]
    if a.all:
        out = os.path.join(REPO, 'build', 'diff')
        os.makedirs(out, exist_ok=True)
        json.dump({os.path.basename(c)[:-5]: ('agreed' if r is True else
                                              'disagreed' if r is False else 'unsupported')
                   for c, r in zip(cases, results)},
                  open(os.path.join(out, 'emu_results.json'), 'w'), indent=1)
    ok = sum(1 for r in results if r is True)
    bad = sum(1 for r in results if r is False)
    skip = sum(1 for r in results if r is None)
    print('\n%d agreed, %d disagreed, %d unsupported' % (ok, bad, skip))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
