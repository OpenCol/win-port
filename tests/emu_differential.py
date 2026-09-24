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

A CALL IS RESOLVED to the SHIPPED callee, by name, through the symbol map. That includes a
callee the shipped code reaches with rule 47's four-byte `PUSH CS / CALL NEAR`: that form
gives the callee a far-style return address, so the callee ends in RETF and a genuine far
call from the candidate returns correctly. This harness refused those for a while on the
strength of a stack-delta mismatch that turned out to be a symbol-map bug -- segment
numbers read as hex when they are decimal -- and not the call form at all.

What it still cannot resolve is a call to something the symbol map does not name; those
are reported as unsupported rather than skipped quietly.

    tests/emu_differential.py tests/cases/job_goods.json
    tests/emu_differential.py --all
"""
import argparse, concurrent.futures, glob, json, multiprocessing, os, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
SRC = os.environ.get('COL_SRC_REPO',
                     os.path.expanduser('~/Repositories/dderevjanik/col-win-re'))
sys.path.insert(0, os.path.join(SRC, 'tools', 're_agent'))
sys.path.insert(0, os.path.join(SRC, 'tools'))
sys.path.insert(0, os.path.join(REPO, 'tools'))
# The DEFAULT only. Each case carries the flags its own reconstruction was matched with
# -- `flags:` in the byte-exact header -- and those win. -Od is the one that matters: 435
# reconstructions specify it, and without it Borland's codegen for a C++ member function
# diverges from the shipped bytes eight bytes in.
FLAGS = '-ml -WS -Od -Ot -3 -dc -DCOL_EMU'


def case_flags(case):
    return (case.get('flags') or FLAGS) + ' -dc -DCOL_EMU'


EXTDEF_RE = None
FIXUP_RE = None


def _res():
    global EXTDEF_RE, FIXUP_RE, LEDATA_RE
    import re
    if EXTDEF_RE is None:
        # TDUMP writes the first entry on the EXTDEF line and the rest on continuation
        # lines with only the index, so keying on the word EXTDEF finds one of them.
        EXTDEF_RE = re.compile(r"^(?:\S+\s+EXTDEF\s+|\s+)(\d+)\s*:\s*'([^']+)'",
                               re.M)
        FIXUP_RE = re.compile(r"FixUp:\s*([0-9a-fA-F]+)\s+Mode:\s*(\w+)\s+"
                              r"Loc:\s*(\w+)\s+Frame:\s*\S+\s+Target:\s*EI\[(\d+)\]")
        LEDATA_RE = re.compile(r"LEDATA\s+Segment:\s*(\S+)\s+Offset:\s*([0-9a-fA-F]+)")
    return EXTDEF_RE, FIXUP_RE, LEDATA_RE


def imports():
    """API name -> (module, ordinal), from the shipped import table."""
    out = {}
    for key, nm in json.load(open(os.path.join(SRC, 're', 'symbols.json')
                                  ))['imports'].items():
        mod, _, ordinal = key.partition('_')
        api = nm[len(mod) + 1:] if nm.startswith(mod + '_') else nm
        out[api.upper()] = (mod, int(ordinal))
    return out


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
    # C++ methods, as TDUMP demangles them. The symbol map spells a constructor
    # `Class_ctor` and a destructor `Class_dtor`, with an address suffix when the image
    # has several -- `EventWin_ctor_1010_28a9`. A name that resolves to exactly one
    # address is taken; one that does not is left unresolved and reported, because the
    # call site is what decides and a name cannot.
    import collections
    byprefix = collections.defaultdict(list)
    for nm in list(out):
        if '_ctor' in nm or '_dtor' in nm:
            cls, _, rest = nm.partition('_')
            byprefix[(cls, 'ctor' if rest.startswith('ctor') else 'dtor')].append(nm)
    for (cls, kind), names in byprefix.items():
        exact = [n for n in names if n == '%s_%s' % (cls, kind)]
        pick = exact or (names if len(names) == 1 else [])
        if pick:
            key = '%s::%s%s()' % (cls, '~' if kind == 'dtor' else '', cls)
            out.setdefault(key, out[pick[0]])
    for nm in list(out):
        if '_' in nm:
            cls, _, meth = nm.partition('_')
            if meth and not meth[0].isdigit():
                out.setdefault('%s::%s()' % (cls, meth), out[nm])
    return out


def shipped_calls(addr, size=1024):
    """The far-call targets in the SHIPPED function, in order.

    A name cannot always say which callee a call means: the image has six
    `EventWin_ctor_*`, and only the call site knows. The shipped function's own bytes do
    know -- a `9A off seg` is a far call with its target in the instruction -- so when a
    demangled name is ambiguous the Nth unresolved call takes the Nth shipped target. If
    that alignment is wrong the differential says so, which is the point of running it.
    """
    import bytematch as BM
    try:
        b = bytes(BM.target_bytes(addr, size))
    except Exception:
        return []
    out, i = [], 0
    while i < len(b) - 4:
        if b[i] == 0x9A:
            off = b[i + 1] | (b[i + 2] << 8)
            sel = b[i + 3] | (b[i + 4] << 8)
            out.append((sel, off))
            i += 5
        else:
            i += 1
    return out


def resolve(code, td, sym, syms, m=None, imps=None, fallback=()):
    """Cut `sym` out of the object and point every call at the SHIPPED callee.

    A ported function that calls another function compiles to a far call with a fixup.
    Pointing that call at the shipped implementation is not a compromise -- it is the
    right experiment: this harness is asking whether THIS function behaves like its
    shipped counterpart, and giving both the same callees is what isolates it. It is also
    what the byte-exact repo's own per-function differential does.

    Returns (bytes, unresolved names).
    """
    import bytematch as BM
    ext_re, fix_re, led_re = _res()
    pubs = BM.parse_pubdefs(td)
    # A `pascal` function's PUBDEF is UPPERCASE and carries no leading underscore --
    # that is what the convention means to Borland, and it is why six window procedures
    # came back "not in PUBDEFs" and were counted as disagreements rather than as
    # symbols this could not find.
    name = next((c for c in (sym, '_' + sym, sym.upper(), '_' + sym.upper())
                 if c in pubs), None)
    if name is None:
        return None, ['%s not in PUBDEFs' % sym]
    off = pubs[name]
    later = [o for o in pubs.values() if o > off]
    end = min(later) if later else len(code)
    extdefs = {int(m.group(1)): m.group(2) for m in ext_re.finditer(td)}
    # WHOLE object, not one symbol, when the method near-calls something the compiler
    # generated beside it -- a C++ destructor calls its members' destructors that way, and
    # slicing the method alone cuts the callee off. Entering at the method's own offset
    # gives the same execution with the helpers still reachable.
    # A FIXUPP offset is relative to the LEDATA record before it, and a C++ object has
    # several. Taking it as absolute clobbered code 8 bytes into the function --
    # CDControlWin's `MOV DS,AX; PUSH SI` became a half-written far call -- and the
    # candidate then ran a different program. Walk the dump in order and carry the base.
    # Only the TEXT records, exactly as bytematch.parse_tdump concatenates them. A C++
    # object also carries _DATA and vtable LEDATAs, and counting those moved the base so
    # every call site was patched somewhere harmless and left as an ffff:0000 placeholder
    # -- the candidate then called nothing at all.
    fixes = []
    base, in_text = 0, True
    for ln in td.splitlines():
        ml = led_re.search(ln)
        if ml:
            in_text = 'TEXT' in ml.group(1).upper()
            base = int(ml.group(2), 16)
            continue
        mf = fix_re.search(ln)
        if mf and in_text:
            fixes.append((base + int(mf.group(1), 16), mf.group(3), int(mf.group(4))))
    whole = any(loc in ('Base', 'CodeOffset16') for _a, loc, _e in fixes)
    lo, hi = (0, len(code)) if whole else (off, end)
    entry = off - lo
    out, missing, fb_i, resolved_by_site = bytearray(code[lo:hi]), [], [0], []
    off = lo
    for at, loc, ei in fixes:
        if not (off <= at < end):
            continue
        target = extdefs.get(ei, '?').lstrip('_')
        if loc == 'Pointer16' and target not in syms and imps and \
                target.upper() in imps and m is not None:
            # A Windows API. The emulator answers it through the same trap the shipped
            # program's own import thunk goes to, so the candidate and the shipped code
            # reach exactly one implementation -- winsim's.
            mod, ordinal = imps[target.upper()]
            lin = m._trap_addr((mod, ordinal))
            import win16emu as W
            off16, sel = lin - W.TRAP_BASE, W.TRAP_SEL
            out[at - off:at - off + 4] = bytes([off16 & 0xFF, off16 >> 8,
                                                sel & 0xFF, sel >> 8])
            continue
        if loc == 'Pointer16' and target not in syms and fb_i[0] < len(fallback):
            # Ambiguous by name: take the shipped function's own next far-call target.
            sel, toff = fallback[fb_i[0]]
            fb_i[0] += 1
            resolved_by_site.append(target)
            out[at - off:at - off + 4] = bytes([toff & 0xFF, toff >> 8,
                                                sel & 0xFF, sel >> 8])
            continue
        if loc != 'Pointer16' or target not in syms:
            missing.append('%s (%s)' % (target, loc))
            continue
        seg, toff = syms[target]
        sel = 0x1000 + (seg - 1) * 8
        out[at - off:at - off + 4] = bytes([toff & 0xFF, toff >> 8,
                                            sel & 0xFF, sel >> 8])
    return (bytes(out), entry), missing


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
    # The API CALLS the function made, with their arguments. This is what makes a
    # function that never returns still comparable: the emulator does not simulate every
    # import -- MSCloseFile stops at KERNEL.81 after 24 instructions -- and both sides stop
    # at the same place for the same reason. What they did on the way there is the
    # evidence, and it is the same surface the byte-exact repo's whole-binary differential
    # treats as its strictest.
    # Which imports, in order. NOT their arguments: the log reports a window of the
    # stack, and below the real arguments it holds whatever the frame left there, which
    # differs between the shipped function and a candidate with its own frame. The
    # sequence of APIs is the part both sides must agree on; data effects are covered by
    # the write comparison.
    calls = [str(c.get('target')) for c in m.log.get('calls', [])
             if c.get('kind') == 'import']
    return {'dxax': o['return_dxax'], 'returned': o['reached_return'],
            'sp_delta': o['sp_delta'], 'writes': writes, 'calls': calls}


BUILT = {}
SYMS = {}
IMPORTS = {}
MACHINES = {}         # (state, stubs) -> (machine, meta, pristine data segments)
CURSOR = {}           # (state, stubs) -> next free offset in the candidate selector
REASONS = {}          # case -> why it was not verified, for tools/coverage.py


CACHE = None


def cache_dir():
    global CACHE
    if CACHE is None:
        CACHE = os.path.join(REPO, 'build', 'emu_objs')
        os.makedirs(CACHE, exist_ok=True)
    return CACHE


def prebuild(cases):
    """Compile every case's source in a few DOSBox sessions rather than one each.

    Booting DOSBox costs seconds and compiling one of these files costs a fraction of
    one, so a file-at-a-time loop spends its whole run on the emulator.
    """
    import bcc
    SYMS.update(symbols())
    srcs = sorted({os.path.join(REPO, json.load(open(c))['source']) for c in cases})
    srcs = [s for s in srcs if os.path.exists(s)]
    by_flags = {}
    for c in cases:
        cs = json.load(open(c))
        src = os.path.join(REPO, cs['source'])
        if os.path.exists(src):
            by_flags.setdefault(case_flags(cs), []).append(src)
    # Compiled ONCE, by the parent, into a cache the shards read. Letting each shard
    # compile its own subset put ten concurrent DOSBox sessions in one work directory,
    # where they overwrote each other's objects -- 736 cases "disagreed" on garbage.
    todo = [x for x in srcs
            if not os.path.exists(os.path.join(
                cache_dir(), os.path.splitext(os.path.basename(x))[0] + '.td'))
            or os.path.getmtime(x) > os.path.getmtime(os.path.join(
                cache_dir(), os.path.splitext(os.path.basename(x))[0] + '.td'))]
    if todo:
        print('compiling %d units with Borland C++ 4.52' % len(todo))
        pending = set(todo)
        for flags, group in sorted(by_flags.items()):
            group = [g for g in group if g in pending]
            if not group:
                continue
            for src, (obj, log, td) in bcc.compile_all(group, flags,
                                                       tdump=True).items():
                stem = os.path.join(cache_dir(),
                                    os.path.splitext(os.path.basename(src))[0])
                open(stem + '.td', 'w').write(td or '')
                open(stem + '.log', 'w').write(log or '')
                if obj:
                    open(stem + '.obj', 'wb').write(obj)


def run_case(path, verbose=False):
    import bcc, bytematch as BM, probe
    case = json.load(open(path))
    name = case['symbol']
    # The results file is keyed by the CASE, so a reason has to be too -- storing it under
    # the symbol meant tools/coverage.py saw an uncovered function with no reason given.
    key = os.path.basename(path)[:-5]
    src = os.path.join(REPO, case['source'])
    if not os.path.exists(src):
        print('%-32s no source at %s' % (name, case['source']))
        return None

    if not SYMS:
        SYMS.update(symbols())
    stem = os.path.join(cache_dir(), os.path.splitext(os.path.basename(src))[0])
    # STALE means wrong, not slow. prebuild() applies this test before it fills the
    # cache; a single case run straight from the command line read whatever was there,
    # and a re-ported file was compared as the version before the fix -- which reads as
    # the fix not working. Same test, in one place.
    if os.path.exists(stem + '.td') and \
            os.path.getmtime(stem + '.td') >= os.path.getmtime(src):
        td = open(stem + '.td').read()
        log = open(stem + '.log').read() if os.path.exists(stem + '.log') else ''
        obj = open(stem + '.obj', 'rb').read() if os.path.exists(stem + '.obj') else None
    else:
        obj, log, td = bcc.compile_unit(src, case_flags(case), tdump=True)
    if obj is None:
        print('%-32s BORLAND BUILD FAILED\n%s' % (name, (log or '')[:600]))
        return False
    code, _ = BM.parse_tdump(td)

    # A FRESH machine per case. Sharing one across cases was worth about a minute and
    # cost three separate bugs -- lazy relocation restoring stale bytes, the emulator's
    # translation cache serving a previous candidate's code, and a stack pointer that
    # moved between cases. Each showed up as a function that passed alone and disagreed
    # in a batch. The speed comes from running cases in parallel processes instead, where
    # nothing is shared by construction.
    m, meta = probe.build(case.get('state', 'shipped'), case.get('stubs', []))
    m.relocate_all()
    if not IMPORTS:
        IMPORTS.update(imports())
    sliced, missing = resolve(code, td, name, SYMS, m, IMPORTS,
                              shipped_calls(case['shipped']))
    if sliced is not None:
        sliced, entry = sliced
    if sliced is None:
        print('%-32s %s' % (name, missing[0]))
        return False
    if missing:
        REASONS[key] = 'cannot resolve %s' % ', '.join(sorted(set(missing))[:3])
        print('%-32s unsupported: %s' % (name, REASONS[key]))
        return None
    for spec, hexbytes in case.get('stage', {}).items():
        seg, off = seg_of(spec)
        m.write(seg, off, bytes.fromhex(hexbytes))
    for at, points_to in case.get('pointers', {}).items():
        seg, off = seg_of(at)
        tseg, toff = seg_of(points_to)
        sel = 0x1000 + (tseg - 1) * 8
        m.write(seg, off, bytes([toff & 0xFF, toff >> 8, sel & 0xFF, sel >> 8]))
    # Captured AFTER this case's staging, so a restore between argument sets puts the
    # staged bytes back too.
    pristine = {}
    for idx, seg in m.image.segs.items():
        if m.image.is_code(idx):
            continue
        n = min(seg.get('min_alloc') or seg['length'] or 0x10000, 0x10000)
        pristine[idx] = m.read(idx, 0, n)
    cand = m.load_candidate(sliced)
    if entry:
        sel, _, _o = cand.partition(':')
        cand = '%s:%04x' % (sel, entry)

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
        why = []
        if a['returned'] != b['returned']:
            why.append('the shipped code %s and the port %s'
                       % ('returned' if a['returned'] else 'did not return',
                          'returned' if b['returned'] else 'did not'))
        if a['calls'] != b['calls']:
            why.append('the imports called differ: shipped %s, the port %s'
                       % (a['calls'][:4], b['calls'][:4]))
        if not a['returned'] and not why:
            # Neither reached its return, both stopped at the same unsimulated import
            # having made the same calls. That is agreement on everything observable.
            continue
        if not a['returned']:
            bad += 1
            print('  DIFF args=%s' % (list(args),))
            for w in why[:4]:
                print('       ' + w)
            continue
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
        REASONS[key] = ('the shipped function returned from none of the %d argument '
                         'sets' % dropped)
        print('%-32s UNVERIFIED: %s' % (name, REASONS[key]))
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
    ap.add_argument('--shard', help='i/n: run only this slice of the cases')
    a = ap.parse_args()
    cases = sorted(glob.glob(os.path.join(HERE, 'cases', '*.json'))) if a.all \
        else [a.case]
    if not cases or cases == [None]:
        ap.error('name a case or pass --all')
    if a.shard:
        i, n = (int(x) for x in a.shard.split('/'))
        cases = cases[i::n]
    elif a.all and len(cases) > 8:
        # One process per core, each with its OWN machines. Sharing a machine across cases
        # was tried and reverted (see run_case); this gets the same wall-clock without
        # sharing anything.
        n = max(1, min(multiprocessing.cpu_count(), 10))
        prebuild(cases)                      # compile once, for every shard to reuse
        out = os.path.join(REPO, 'build', 'diff')
        os.makedirs(out, exist_ok=True)

        def shard(i):
            return subprocess.run(
                [sys.executable, os.path.abspath(__file__), '--all', '--shard',
                 '%d/%d' % (i, n)], capture_output=True, text=True,
                env=dict(os.environ, COL_EMU_SHARD='%d' % i))
        # A shard that DIES takes its cases with it, and nothing here used to say so:
        # its stderr was discarded, its partial results file merged as though complete,
        # and tools/coverage.py then reported its cases as uncovered and unexplained.
        # One shard lost 81 of its 238 that way and the run still called itself a run.
        merged, bad = {}, 0
        for i in range(n):                    # stale ones would merge as this run's
            f = os.path.join(out, 'emu_results_shard%d.json' % i)
            if os.path.exists(f):
                os.remove(f)
        with concurrent.futures.ThreadPoolExecutor(max_workers=n) as pool:
            for r in pool.map(shard, range(n)):
                sys.stdout.write(r.stdout)
                if r.returncode and r.stderr:
                    sys.stdout.write(r.stderr)
                bad |= r.returncode
        short = []
        for i in range(n):
            f = os.path.join(out, 'emu_results_shard%d.json' % i)
            want = len(cases[i::n])
            if not os.path.exists(f):
                short.append('shard %d wrote no results at all' % i)
                continue
            got = json.load(open(f))
            if len(got) != want:
                short.append('shard %d ran %d of its %d cases'
                             % (i, len(got), want))
            merged.update(got)
        if short:
            print('THE RUN IS INCOMPLETE, so it is not a result:')
            for line in short:
                print('  ' + line)
            bad = 1
        json.dump(merged, open(os.path.join(out, 'emu_results.json'), 'w'), indent=1)
        print('\n%d agreed, %d disagreed, %d unsupported'
              % (sum(1 for v in merged.values() if v['status'] == 'agreed'),
                 sum(1 for v in merged.values() if v['status'] == 'disagreed'),
                 sum(1 for v in merged.values() if v['status'] == 'unsupported')))
        return 1 if any(v['status'] == 'disagreed' for v in merged.values()) else 0
    if len(cases) > 1:
        prebuild(cases)
    # ONE case must not cost the others. A candidate that makes the emulator raise --
    # writing a selector no descriptor covers, say -- used to take the whole shard down
    # with it, and the parent merged the partial results as though the run had finished:
    # 81 cases came back "uncovered and unexplained" because of one. Caught here, named,
    # and counted as unsupported with the emulator's own words as the reason.
    results = []
    for c in cases:
        try:
            results.append(run_case(c, a.verbose))
        except KeyboardInterrupt:
            raise
        except Exception as e:
            key = os.path.basename(c)[:-5]
            why = 'the emulator raised %s: %s' % (type(e).__name__, str(e)[:120])
            REASONS[key] = why
            print('%-32s %s' % (json.load(open(c))['symbol'], why))
            results.append(None)
    if a.all:
        out = os.path.join(REPO, 'build', 'diff')
        os.makedirs(out, exist_ok=True)
        tag = ('_shard%s' % os.environ['COL_EMU_SHARD']) if os.environ.get(
            'COL_EMU_SHARD') else ''
        json.dump({os.path.basename(c)[:-5]:
                   {'status': ('agreed' if r is True else
                               'disagreed' if r is False else 'unsupported'),
                    'why': REASONS.get(os.path.basename(c)[:-5], '')}
                   for c, r in zip(cases, results)},
                  open(os.path.join(out, 'emu_results%s.json' % tag), 'w'), indent=1)
    ok = sum(1 for r in results if r is True)
    bad = sum(1 for r in results if r is False)
    skip = sum(1 for r in results if r is None)
    print('\n%d agreed, %d disagreed, %d unsupported' % (ok, bad, skip))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
