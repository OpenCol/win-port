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


def prebuild(cases):
    """Compile every case's source in a few DOSBox sessions rather than one each.

    Booting DOSBox costs seconds and compiling one of these files costs a fraction of
    one, so a file-at-a-time loop spends its whole run on the emulator.
    """
    import bcc
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

    if src in BUILT:
        obj, log, td = BUILT[src]
    else:
        obj, log, td = bcc.compile_unit(src, FLAGS, tdump=True)
    if obj is None:
        print('%-32s BORLAND BUILD FAILED\n%s' % (name, (log or '')[:600]))
        return False
    code, fixups = BM.parse_tdump(td)
    sliced, sf = BM.slice_symbol(code, fixups, td, name, 512)
    if sliced is None:
        print('%-32s not in the object\'s PUBDEFs' % name)
        return False
    if sf:
        print('%-32s unsupported: %d fixup bytes -- it calls something, and nothing here '
              'resolves a call' % (name, len(sf)))
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
    bits = case.get('ret_bits', 16)
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
    ok = sum(1 for r in results if r is True)
    bad = sum(1 for r in results if r is False)
    skip = sum(1 for r in results if r is None)
    print('\n%d agreed, %d disagreed, %d unsupported' % (ok, bad, skip))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
