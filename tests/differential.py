#!/usr/bin/env python3
"""Does a PORTED function do what the SHIPPED function does?

This repo gave up the byte match, which was what stopped a wrong reconstruction shipping.
This is the replacement, and it has to exist before the porting does. Both sides are run
over the same argument sweep from the same staged memory, and two things are compared:

  the return value        masked to the width the function actually returns
  the memory afterwards   at every address the shipped function wrote, plus the
                          requirement that the port wrote NOWHERE ELSE

The shipped side runs in the byte-exact repo's emulator (tests/shipped_probe.py, under
that repo's interpreter -- it owns the emulator and this repo does not fork it). The host
side is the ported C, compiled by a host compiler with the model's widths and byte packing
and every global placed at its shipped address, so the two agree on coordinates.

WHAT THIS CANNOT SEE, stated so a pass is not read as more than it is:

  - reads. The host has no MMU; it diffs slabs. A port that reads the wrong address but
    computes the right answer passes here. The byte-exact repo's own layer B does see
    reads, and a function whose result depends on what it read will fail here anyway.
  - write ORDER, and a write of a value that was already there.
  - anything above one function: calling convention, stack discipline, segment arithmetic.
    Those are layer A's (a whole-binary differential boot), which needs a linked .EXE.

    tests/differential.py tests/cases/clear_colony_flag_1c.json
    tests/differential.py --all
"""
import argparse, glob, json, os, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
SRC_REPO = os.environ.get('COL_SRC_REPO',
                          os.path.expanduser('~/Repositories/dderevjanik/col-win-re'))
UC = os.path.join(SRC_REPO, 'vendor', 're-agent-venv', 'bin', 'python')
BUILD = os.path.join(REPO, 'build', 'diff')
CC = os.environ.get('CC', 'cc')
# -fsigned-char   Borland's default, and the program depends on it: signedness is what
#                 decides JL from JB, and macOS/arm64 clang defaults the other way.
# -fwrapv         Borland wraps signed overflow; C says it is undefined.
# -fno-strict-aliasing  every global here is a cast of the same byte slab.
LIB = None
# Functions the host cannot judge, with the reason, each required to pass the 16-bit
# harness instead. Listed rather than excused: see model/host_blind.json.
HOST_BLIND = json.load(open(os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), 'model', 'host_blind.json')))['blind']
CFLAGS = ['-std=c89', '-DCOL_HOST=1', '-fsigned-char', '-fwrapv', '-fno-strict-aliasing',
          '-Wall', '-Wno-unused-value', '-g', '-O1']


BROKEN = {}


def archive():
    """Compile every ported source once and archive it, so a case can call a callee.

    A ported function may call another ported function, and linking only the case's own
    file leaves that undefined. Compiling all of them per case would cost minutes over a
    run, so they are compiled once into build/diff/libcol.a and each case links against
    it -- the driver is then the only file that changes between cases.

    A source that will not compile is reported once and left out; its own case then fails
    to link, which is the honest outcome, and every other case still runs.
    """
    objdir = os.path.join(BUILD, 'obj')
    os.makedirs(objdir, exist_ok=True)
    lib = os.path.join(BUILD, 'libcol.a')
    objs, sources = [], sorted(glob.glob(os.path.join(REPO, 'src', '*', '*.c'))) + \
        [os.path.join(REPO, 'src', 'segdata.c'), os.path.join(HERE, 'host', 'colmem.c')]
    stale = False
    for src in sources:
        obj = os.path.join(objdir, os.path.basename(src)[:-2] + '.o')
        if not os.path.exists(obj) or os.path.getmtime(obj) < os.path.getmtime(src):
            r = subprocess.run([CC] + CFLAGS + ['-I' + os.path.join(REPO, 'include'),
                                                '-c', src, '-o', obj],
                               capture_output=True, text=True)
            if r.returncode:
                BROKEN[os.path.basename(src)] = r.stderr[:2000]
                continue
            stale = True
        objs.append(obj)
    if stale or not os.path.exists(lib):
        subprocess.run(['ar', 'rcs', lib] + objs, check=True)
    return lib


def sweep(case):
    sys.path.insert(0, os.path.join(SRC_REPO, 'tools', 're_agent'))
    import probe
    return probe.parse_sweep(case.get('sweep', []), case.get('call_args', []))


def seg_of(spec):
    where, _, off = spec.partition(':')
    return (34 if where == 'DGROUP' else int(where[3:])), int(off, 0)


def driver(case, argsets, probe_addrs, poison):
    """The host program: stage, snapshot, call, print what moved. Generated, not kept,
    so it cannot drift from the case file it came from."""
    nargs = max((len(a) for a in argsets), default=0)
    rows = ',\n    '.join('{' + ', '.join('%dL' % v for v in
                                          list(a) + [0] * (nargs - len(a))) + '}'
                          for a in argsets)
    stage = []
    for spec, hexbytes in case.get('stage', {}).items():
        seg, off = seg_of(spec)
        b = bytes.fromhex(hexbytes)
        stage.append('        {\n            static unsigned char b[] = {%s};\n'
                     '            memcpy(col_seg(%d) + 0x%04x, b, sizeof b);\n        }'
                     % (', '.join('0x%02x' % x for x in b), seg, off))
    for at, points_to in case.get('pointers', {}).items():
        stage.append('        /* %s -> %s is staged by name below: a far pointer is four\n'
                     '           bytes in the game and a host pointer here, so the two\n'
                     '           sides stage it differently ON PURPOSE. */' % (at, points_to))
    for line in case.get('host_stage', []):
        stage.append('        ' + line)
    probes = ',\n    '.join('{%d, 0x%04xL}' % a for a in probe_addrs) or '{0, 0}'
    pois = ',\n    '.join(
        '{' + ','.join('{%d, 0x%04xL, %d}' % (int(a.split(':')[0]), int(a.split(':')[1], 16), v)
                       for a, v in sorted(p.items())) + '}' if p else '{{0, 0, 0}}'
        for p in poison)
    npois = max((len(p) for p in poison), default=0)
    return '''/* GENERATED by tests/differential.py from %s -- do not edit, do not keep. */
#include <stdio.h>
#include <string.h>
#include "col/globals.h"
#include "col/colmem.h"

/* The emulator's data segments after ITS startup has run, handed over rather than
 * approximated: see tests/shipped_probe.py. */
static void col_load_state(const char *path)
{
    FILE *f = fopen(path, "rb");
    unsigned short seg;
    unsigned long n;
    if (!f)
        return;
    while (fread(&seg, 2, 1, f) == 1 && fread(&n, 4, 1, f) == 1) {
        unsigned char *p = col_seg((int)seg);
        if (p && n <= (unsigned long)col_seg_size((int)seg))
            fread(p, 1, (size_t)n, f);
        else
            fseek(f, (long)n, SEEK_CUR);
    }
    fclose(f);
}

%s

#define NARGS %d
#define NSETS %d
static long argsets[NSETS][NARGS > 0 ? NARGS : 1] = {
    %s
};

/* Every address the SHIPPED function wrote, over any argument set. The host reports its
 * value at each one whether or not it CHANGED: a write of a value that was already there
 * is invisible to a diff, and the shipped code writes whole words whose high byte is
 * often already zero. Comparing values rather than changes removes that whole class of
 * false difference. */
#define NPROBE %d
static struct { int seg; long off; } probes[NPROBE > 0 ? NPROBE : 1] = {
    %s
};

/* The same poison the shipped side was given before the call it was measured on, so a
 * write of a value that was already there still changes something here. */
#define NPOISON %d
static struct { int seg; long off; int val; }
    poison[NSETS][NPOISON > 0 ? NPOISON : 1] = {
    %s
};

int main(int argc, char **argv)
{
    int i, n, k;
    if (argc > 1)
        col_load_state(argv[1]);
    col_capture();          /* this, not the linked image, is what col_reset() restores */
    static int segs[4096];
    static unsigned offs[4096];
    static unsigned char vals[4096];
    printf("{\\"observations\\":[");
    for (i = 0; i < NSETS; i++) {
%s
        long r;
%s
        col_reset();
%s
        /* AFTER staging, exactly as the shipped side does it: the shipped probe captures
           its pristine image once the case's staging is in place and poisons on top. */
        for (k = 0; k < NPOISON; k++)
            if (poison[i][k].seg)
                col_seg(poison[i][k].seg)[poison[i][k].off] =
                    (unsigned char)poison[i][k].val;
        col_snapshot();
        %s;
        n = col_diff(segs, offs, vals, 4096);
        printf("%%s{\\"args\\":[", i ? "," : "");
        for (k = 0; k < NARGS; k++)
            printf("%%s%%ld", k ? "," : "", argsets[i][k]);
        printf("], \\"ret\\":%%ld, \\"at\\":{", r);
        for (k = 0; k < NPROBE; k++)
            printf("%%s\\"%%d:%%04lx\\":%%d", k ? "," : "", probes[k].seg,
                   probes[k].off, col_seg(probes[k].seg)[probes[k].off]);
        printf("}, \\"changed\\":{");
        for (k = 0; k < n && k < 4096; k++)
            printf("%%s\\"%%d:%%04x\\":%%d", k ? "," : "", segs[k], offs[k], vals[k]);
        printf("}}");
    }
    printf("]}\\n");
    return 0;
}
''' % (os.path.basename(case['_path']), case['prototype'], nargs, len(argsets), rows,
       len(probe_addrs), probes, npois, pois,
       # one `aK` per argument WORD, not a fixed four: a pointer argument takes two
       # words, so a four-argument function can reach a4.
       '        long ' + ', '.join('a%d = argsets[i][%d]' % (k, k)
                                   for k in range(max(nargs, 1))) + ';'
       if nargs else '        long a0 = 0;',
       '\n'.join('        (void)a%d;' % k for k in range(max(nargs, 1))),
       '\n'.join(stage),
       ('r = ' + case['call']) if case.get('ret_bits', 16) else
       ('(void)(' + case['call'] + '), r = 0'))


# A comparison whose null case cannot fail measures nothing. Each mutation below is a
# wrong port of a KIND this harness claims to catch; --selftest requires every one of them
# to be reported as a difference, on every case. The pair is deliberate: one wrong answer,
# one right answer written to the wrong place.
MUTATIONS = {
    'off-by-one return': ('(%s) + 1', ''),
    'a write the shipped code does not make':
        ('%s', '        col_seg(20)[0x2000] = 0x5a;\n'),
    # A void function has no return value to spoil, so the first mutation is a no-op on it
    # and the selftest would pass without testing anything. 43 cases were in that state.
    'the function never runs': (None, ''),
}


def applicable(case):
    bits = case.get('ret_bits', 16)
    return [m for m in MUTATIONS
            if (m != 'off-by-one return' or bits)
            and (m != 'the function never runs' or not bits)]


def run_case(path, verbose=False, mutate=None):
    case = json.load(open(path))
    if case['symbol'] in HOST_BLIND and not mutate:
        # Checked BEFORE anything runs: find_entry's loop bound comes out of the staged
        # region and walks off the end of the slab, so running it at all is a fault.
        print('%-32s UNVERIFIED BY HOST: %s'
              % (case['symbol'], HOST_BLIND[case['symbol']]))
        return None
    case['_path'] = path
    name = case['symbol']
    argsets = sweep(case)
    os.makedirs(BUILD, exist_ok=True)
    if not os.path.exists(UC):
        print('%s: no emulator interpreter at %s -- run tools/re_agent/setup.sh there'
              % (name, UC))
        return False
    # The shipped side runs FIRST, because what it wrote decides what the host is asked
    # about.
    dump = os.path.join(BUILD, name + '.state')
    r = subprocess.run([UC, os.path.join(HERE, 'shipped_probe.py'), path, SRC_REPO, dump],
                       capture_output=True, text=True)
    if r.returncode:
        print('%s: SHIPPED PROBE FAILED\n%s' % (name, r.stderr[-3000:]))
        return False
    ship = json.loads(r.stdout)
    written = sorted({tuple(int(x, 16 if ':' in a else 10) for x in
                            (a.split(':')[0], a.split(':')[1]))
                      for o in ship['observations'] for a in o['final']},
                     key=lambda t: t)
    probe_addrs = [(int(a.split(':')[0]), int(a.split(':')[1], 16))
                   for a in sorted({a for o in ship['observations'] for a in o['final']})]

    # Argument sets the SHIPPED function never returned from are dropped before the host
    # is built, not after: a chain walk over data that happens to be cyclic does not
    # terminate on a host either, and there is nothing to compare it against anyway.
    keep = [i for i, o in enumerate(ship['observations']) if o.get('returned', True)]
    skipped = len(ship['observations']) - len(keep)
    ship['observations'] = [ship['observations'][i] for i in keep]
    argsets = [argsets[i] for i in keep]
    if not argsets:
        # Not a disagreement: there is no answer to disagree with. fill_unless walks a far
        # pointer until the POINTER is zero, which it never is once the selector is set,
        # so the shipped function does not terminate for any argument but a null one.
        print('%-32s UNVERIFIED: the shipped function returned from no argument set in '
              'this sweep' % name)
        return None
    drv = os.path.join(BUILD, name + '_drv.c')
    text = driver(case, argsets, probe_addrs,
                  [o.get('poison', {}) for o in ship['observations']])
    if mutate:
        call, extra = MUTATIONS[mutate]
        before = text
        if call is None:
            text = text.replace('(void)(%s), r = 0;' % case['call'], 'r = 0;')
        elif 'r = %s;' % case['call'] in text:
            text = text.replace('r = %s;' % case['call'], 'r = %s;' % (call % case['call']))
        elif '(void)(%s)' % case['call'] in text:
            text = text.replace('(void)(%s)' % case['call'],
                                '(void)(%s)' % (call % case['call']))
        else:
            text = text.replace('%s;' % case['call'], '%s;' % (call % case['call']))
        if extra:
            text = text.replace('        n = col_diff(', extra + '        n = col_diff(')
        if text == before:
            print('%s: the %s mutation changed nothing -- the selftest would be a lie'
                  % (name, mutate))
            return True          # reported as NOT caught
    open(drv, 'w').write(text)
    exe = os.path.join(BUILD, name)
    if os.path.basename(case['source']) in BROKEN:
        print('%s: HOST BUILD FAILED\n%s'
              % (name, BROKEN[os.path.basename(case['source'])]))
        return False
    cmd = [CC] + CFLAGS + ['-I' + os.path.join(REPO, 'include'), '-o', exe, drv, LIB]
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode:
        print('%s: HOST BUILD FAILED\n%s' % (name, r.stderr[:3000]))
        return False
    # A wrong port can fault or spin. Neither may take the harness down with it: the
    # whole point is to run every case and say which ones disagree.
    try:
        r = subprocess.run([exe, dump], capture_output=True, text=True, timeout=60)
    except subprocess.TimeoutExpired:
        print('%s: the port did not finish in 60s' % name)
        return False
    if r.returncode != 0 or not r.stdout.strip():
        print('%s: the port died (%s)%s' % (name, r.returncode,
                                            '\n  ' + r.stderr.strip()[:300]
                                            if r.stderr.strip() else ''))
        return False
    host = json.loads(r.stdout)

    bits = case.get('ret_bits', 16)
    mask = (1 << bits) - 1 if bits else 0
    # A case that compares no return value and observes no write compares NOTHING, and
    # would sit in the suite looking green. Three ports were in that state: the sweep never
    # reached the branch that writes. Say so instead.
    if not bits and not any(o['final'] for o in ship['observations']):
        if case.get('unverifiable'):
            print('%-32s UNVERIFIED: %s' % (name, case['unverifiable']))
            return None
        # Not a failure: a function can have no effect this harness can see -- an empty
        # one, or one whose whole effect is a call. Whether anything covers it is a
        # question for tools/coverage.py, which reads both harnesses.
        print('%-32s UNVERIFIED BY HOST: the shipped code neither returned a value nor '
              'wrote, over this sweep' % name)
        return None
    bad = 0
    for h, s_ in zip(host['observations'], ship['observations']):
        why = []
        shipped_ret = s_['dxax'] if bits > 16 else s_['ax']
        if mask and (h['ret'] & mask) != (shipped_ret & mask):
            why.append('returned 0x%x, shipped 0x%x (%d bits)'
                       % (h['ret'] & mask, shipped_ret & mask, bits))
        for addr, val in s_['final'].items():
            hv = h['at'].get(addr)
            if hv is None:
                why.append('%s: not probed (a harness bug, not a port bug)' % addr)
            elif hv != val:
                why.append('%s: shipped holds %d, the port holds %d' % (addr, val, hv))
        for addr in h['changed']:
            if addr not in s_['final']:
                why.append('%s: the port wrote here and the shipped code did not' % addr)
        if why and name in HOST_BLIND:
            continue
        if why:
            bad += 1
            print('  DIFF args=%s' % (h['args'],))
            for w in why:
                print('       ' + w)
        elif verbose:
            print('  ok   args=%s ret=0x%x, %d bytes agree'
                  % (h['args'], h['ret'] & mask, len(s_['final'])))
    if not mutate:
        if skipped:
            print('%-32s %d argument set%s dropped: the shipped function did not return'
                  % (name, skipped, '' if skipped == 1 else 's'))
        print('%-32s %d/%d argument sets agree (state: %s)'
              % (name, len(host['observations']) - bad, len(host['observations']),
                 ship['state']))
    return bad == 0


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('case', nargs='?')
    ap.add_argument('--all', action='store_true')
    ap.add_argument('-v', '--verbose', action='store_true')
    ap.add_argument('--selftest', action='store_true',
                    help='break each port on purpose and require this to notice')
    a = ap.parse_args()
    global LIB
    LIB = archive()
    cases = sorted(glob.glob(os.path.join(HERE, 'cases', '*.json'))) if a.all \
        else [a.case]
    if not cases or cases == [None]:
        ap.error('name a case or pass --all')
    if a.selftest:
        ok = True
        for c in cases:
            for name in applicable(json.load(open(c))):
                caught = not run_case(c, False, mutate=name)
                print('%-32s %s: %s' % (os.path.basename(c), name,
                                        'caught' if caught else 'NOT CAUGHT'))
                ok = ok and caught
        return 0 if ok else 1
    results = [run_case(c, a.verbose) for c in cases]
    if a.all:
        json.dump({os.path.basename(c)[:-5]: ('agreed' if r is True else
                                              'disagreed' if r is False else 'unverified')
                   for c, r in zip(cases, results)},
                  open(os.path.join(BUILD, 'host_results.json'), 'w'), indent=1)
    unver = sum(1 for r in results if r is None)
    if unver:
        print('\n%d case%s unverified: no oracle covers them, and each says why'
              % (unver, '' if unver == 1 else 's'))
    return 0 if all(r is not False for r in results) else 1


if __name__ == '__main__':
    sys.exit(main())
