#!/usr/bin/env python3
"""Move one reconstruction from the byte-exact repo into this one, and write its case.

A reconstruction is a standalone translation unit: it re-declares every type it touches
and every global it reads, because a byte-exact file cannot include a shared header. Here
the shared header exists, so the port is mostly DELETION -- and deletion is where a port
quietly changes meaning, so nothing is deleted on a guess.

WHAT IT WILL DO

  * drop every `extern` whose name col/globals.h already declares, after checking the
    address the two agree on;
  * drop `far`, `near` and `huge`, which are addressing modes with no semantic here;
  * keep `pascal` and `cdecl`, which are NOT: they decide argument order and who cleans
    the stack, and a callback Windows invokes has to keep the one it was written with;
  * write tests/cases/<name>.json so the port is compared against the shipped bytes
    before anyone believes it.

WHAT IT WILL REFUSE, rather than guess

  * a file that declares its own struct (`--tier b` is not written yet: the member names
    have to be mapped onto the merged record by OFFSET, and a wrongly named receiver
    compiles identically, so that needs its own checking);
  * an `extern` this repo's model does not know, or knows at a different address;
  * a pointer argument or return, which the sweep cannot generate values for;
  * `pascal` with more than one argument -- the emulator pushes right to left, so the
    order is only unobservable when there is nothing to order.

A refusal names the reason and writes nothing.

    tools/port.py <path in the byte-exact repo>      # one file
    tools/port.py --survey                           # what it would take, by reason
    tools/port.py --batch 20                         # port up to 20 it can do
"""
import argparse, glob, json, os, re, sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
SRC_REPO = os.environ.get('COL_SRC_REPO',
                          os.path.expanduser('~/Repositories/dderevjanik/col-win-re'))
DERIVED = json.load(open(os.path.join(REPO, 'model', 'derived.json')))
GLOBALS = DERIVED['globals']
ALIASES = {a['name'] for a in DERIVED['aliases']}
SEGSIZE = {int(k): v for k, v in
           json.load(open(os.path.join(REPO, 'model', 'segments.json')))['sizes'].items()}

QUALS = ('far', 'near', 'huge', '_far', '_near', '_huge', '__far', '__near', '__huge')
CONV = ('pascal', 'cdecl', '_pascal', '_cdecl', '__pascal', '__cdecl')
INTEGRAL = {'char': 8, 'signed char': 8, 'unsigned char': 8, 'short': 16,
            'unsigned short': 16, 'int': 16, 'unsigned int': 16, 'unsigned': 16,
            'long': 32, 'unsigned long': 32, 'void': 0}
HEADER = re.compile(r'\A\s*/\*.*?\*/', re.S)
# Stops AT the semicolon on purpose: the trailing comment is swallowed afterwards by
# offset, because it can run onto later lines and `.*$` would take only the first of them.
EXTERN = re.compile(r'^[ \t]*extern\b[^;]*;', re.M)
TYPEDEF = re.compile(r'\btypedef\b')
ADDR = re.compile(r'\b((?:SEG\d+|DGROUP)\s*[: ]\s*(?:0x)?[0-9a-fA-F]+)')
DEF = re.compile(r'^(?P<head>[A-Za-z_][\w\s\*]*?)(?P<name>\w+)\s*\((?P<args>[^;{]*)\)\s*\{',
                 re.M)


def blank_comments(text):
    """The text with every comment replaced by spaces, offsets preserved.

    Everything this tool searches for -- externs, typedefs, the definition, the calls --
    must be searched for HERE and cut out of the original by offset. A prose comment that
    happens to contain the word `typedef`, or an extern whose trailing comment runs onto
    the next three lines, is otherwise read as code; the second of those quietly left the
    tail of a comment behind in a ported file.
    """
    out, i, n = [], 0, len(text)
    while i < n:
        if text.startswith('/*', i):
            j = text.find('*/', i + 2)
            j = n if j < 0 else j + 2
            out.append(''.join(c if c == '\n' else ' ' for c in text[i:j]))
            i = j
        elif text.startswith('//', i):
            j = text.find('\n', i)
            j = n if j < 0 else j
            out.append(' ' * (j - i))
            i = j
        else:
            out.append(text[i])
            i += 1
    return ''.join(out)


def strip_quals(text):
    for q in QUALS:
        text = re.sub(r'\b%s\b' % q, ' ', text)
    return re.sub(r'[ \t]+', ' ', text)


def normalise_addr(s):
    s = s.replace(' ', ':').replace('::', ':')
    where, _, off = s.partition(':')
    return '%s:0x%04x' % (where, int(off, 16))


def parse_params(args):
    """[(type, name)] for a parameter list, or None if it has something unsweepable."""
    args = strip_quals(args).strip()
    if args in ('', 'void'):
        return []
    out = []
    for part in args.split(','):
        part = part.strip()
        if '*' in part or '[' in part:
            return None
        words = part.split()
        if len(words) < 2:
            return None
        name, ty = words[-1], ' '.join(words[:-1])
        if ty not in INTEGRAL or INTEGRAL[ty] == 0:
            return None
        out.append((ty, name))
    return out


def index_bound(rest, params, names):
    """How far argument 0 may be swept before it indexes past a table, or None.

    A sweep that runs off the end of a global array is a harness bug rather than a port
    bug, and it does not look like one: on the host the write lands in whatever the linker
    put next, while the emulator's selector simply ends, so the two disagree and the
    disagreement says nothing about the port. The model knows each table's extent, so ask
    it. Only the UPPER end needs bounding -- a negative index still lands inside the
    segment image, where both sides read the same bytes and agree.
    """
    if not params:
        return None
    p0 = params[0][1]
    best = None
    for n in names:
        g = GLOBALS.get(n)
        if not g or not g['dims'] or not re.search(r'\b%s\s*\[\s*%s\b'
                                                   % (re.escape(n), re.escape(p0)), rest):
            continue
        rec = DERIVED.get('records', {})
        width = {'char': 1, 'signed char': 1, 'unsigned char': 1, 'col_i8': 1,
                 'short': 2, 'int': 2, 'unsigned': 2, 'unsigned int': 2,
                 'unsigned short': 2, 'long': 4, 'unsigned long': 4}.get(g['base'])
        if g['stars']:
            width = 4
        if not width:
            continue
        for d in g['dims'][1:]:
            width *= int(d, 0)
        # NOT the global's own extent. 360 of those are inferred from the gap to the next
        # NAMED global, and a table whose later slots were named separately -- the text
        # argument array is exactly that -- would be cut to one element. Running into the
        # next global is harmless: it is the same segment image on both sides, and the
        # emulator logs the write like any other. Running off the SEGMENT is what breaks.
        count = (SEGSIZE[g['seg']] - g['off']) // width
        best = count - 1 if best is None else min(best, count - 1)
    return best


def sweep_for(params):
    """A sweep that exercises each argument, kept under 64 calls so a case stays cheap."""
    n = len(params)
    if n == 0:
        return [], [()]
    if n == 1:
        return ['0=-4..24'], None
    if n == 2:
        return ['0=0..7', '1=0..7'], None
    if n == 3:
        return ['0=0..3', '1=0..3', '2=0..3'], None
    return None, None


def port(path, force=False):
    """(source text, case dict) or (None, reason)."""
    name = os.path.splitext(os.path.basename(path))[0]
    text = open(path, errors='replace').read()
    m = HEADER.match(text)
    head, body = (m.group(0), text[m.end():]) if m else ('', text)

    blank = blank_comments(body)
    if TYPEDEF.search(blank):
        return None, 'declares its own struct (tier B, not written yet)'

    shipped = re.search(r'candidate for (\w{4}:\w{4})', head)
    if not shipped:
        return None, 'no shipped address in the header comment'

    # Cut each extern out by OFFSET, taking the trailing comment with it -- it may run
    # onto later lines, and leaving its tail behind is not a compile error in the source
    # repo's spelling but is one here.
    spans, dropped = [], []
    for m in EXTERN.finditer(blank):
        start, end = m.start(), m.end()
        while end < len(body) and body[end] in ' \t':
            end += 1
        if body.startswith('/*', end):
            j = body.find('*/', end + 2)
            end = len(body) if j < 0 else j + 2
        spans.append((start, end))
    cut = []
    for start, end in spans:
        line = body[start:end]
        names = re.findall(r'\b(g_\w+)\b', blank[start:end])
        if not names:
            # An extern FUNCTION: the prototype for a callee. It stays -- this repo has no
            # header of ported prototypes yet, and the call check above has already
            # refused the file unless that callee is ported.
            continue
        for n in names:
            if n not in GLOBALS and n not in ALIASES:
                return None, 'the model does not know the global %s' % n
            want = ADDR.search(line)
            if want and n in GLOBALS:
                a, b = normalise_addr(want.group(1)), GLOBALS[n]['addr']
                if a != b:
                    return None, '%s: the file says %s, the model says %s' % (n, a, b)
        # Keep only the declaration: the trailing /* address */ would close the comment
        # this line is about to be quoted inside, and the model's address is authoritative
        # anyway.
        dropped.append((re.split(r'/\*', line.strip())[0].strip(), names))
        cut.append((start, end))

    rest, blank_rest, at = '', '', 0
    for start, end in cut:
        rest += body[at:start]
        blank_rest += blank[at:start]
        at = end
    rest, blank_rest = rest + body[at:], blank_rest + blank[at:]
    d = DEF.search(blank_rest)
    if not d:
        return None, 'no function definition found'
    if d.group('name') != name:
        return None, 'defines %s, not %s' % (d.group('name'), name)
    if len(DEF.findall(blank_rest)) > 1:
        return None, 'defines more than one function (an in-file stub)'

    # A callee this repo has not ported yet would not link, and porting it blind is how a
    # port acquires a function nobody checked. Decline, and the file becomes portable the
    # moment its callee is.
    KEYWORDS = {'if', 'while', 'for', 'switch', 'return', 'sizeof', 'do', 'else'}
    called = {c for c in re.findall(r'\b([A-Za-z_]\w*)\s*\(', blank_rest)
              if c not in KEYWORDS and c != name}
    missing = sorted(c for c in called
                     if not glob.glob(os.path.join(REPO, 'src', '*', c + '.c')))
    if missing:
        return None, 'calls %s, which is not ported' % ', '.join(missing[:3])

    params = parse_params(d.group('args'))
    if params and any(p[0].endswith('long') for p in params):
        # The sweep hands the emulator one 16-bit word per argument; a long takes two, so
        # every argument after it would land on the wrong stack slot. The disagreement
        # would be the harness's, not the port's.
        return None, 'a long argument, which the sweep passes as one word'
    if params is None:
        return None, 'a parameter this cannot generate values for'
    conv = [c for c in CONV if re.search(r'\b%s\b' % c, d.group('head'))]
    if conv and len(params) > 1:
        return None, '%s with %d arguments: the emulator pushes the other way' \
            % (conv[0], len(params))
    # A far pointer is four bytes in the game and eight on the host, so a global that
    # HOLDS one cannot be compared: the host store writes four bytes too many. Reading a
    # pointer global is fine; only writing one is not.
    # A pointer-valued global holds a 16:16 far pointer, which is four bytes. A host
    # pointer is eight, so on the host that global's stored value is an ENCODING, not an
    # address: storing through it writes four bytes too many, and following it faults.
    # Reading, writing and dereferencing are all unsafe, so the whole global is.
    for _, names in dropped:
        for n in names:
            if GLOBALS.get(n, {}).get('stars'):
                return None, 'uses the pointer global %s, which is 4 bytes in the game ' \
                             'and 8 on the host' % n

    if re.search(r'\(\s*\*\s*g_\w+\s*\)\s*\(', blank_rest):
        return None, 'calls through a global function pointer, which the model types as ' \
                     'data'

    sweeps, calls = sweep_for(params)
    bound = index_bound(rest, params, [n for _, ns in dropped for n in ns])
    if sweeps and bound is not None and bound >= 0:
        lo, _, hi = sweeps[0].partition('=')[2].partition('..')
        sweeps[0] = '0=%s..%d' % (lo, min(int(hi), bound))
    if sweeps is None:
        return None, '%d arguments is more than a sweep should enumerate' % len(params)

    ret = strip_quals(d.group('head')).strip()
    for c in CONV:
        ret = re.sub(r'\b%s\b' % c, '', ret).strip()
    if ret not in INTEGRAL:
        return None, 'returns %r, which the comparison cannot mask' % ret
    if INTEGRAL[ret] == 0 and not dropped:
        # Nothing to compare: no return value to mask and no global it could have written.
        return None, 'returns void and touches no global this repo knows'

    # col/globals.h gives an array-shaped global a POINTER form, because a global one file
    # indexes and another reads whole is indexable in the header and scalar at the site.
    # Rewrite the scalar uses; a use that takes its ADDRESS has no such rewrite, so refuse.
    body_text = rest
    for _, names in dropped:
        for n in names:
            if not GLOBALS.get(n, {}).get('dims'):
                continue
            if re.search(r'&\s*%s\b' % re.escape(n), blank_rest):
                return None, 'takes the address of %s, which the header spells as a ' \
                             'pointer' % n
            body_text = re.sub(r'\b%s\b(?!\s*\[)' % re.escape(n), '(*%s)' % n, body_text)

    # A function that returns a local it never assigned is undefined, and the shipped
    # answer is whatever DX:AX happened to hold -- kilo_shift_stub is a stubbed-out routine
    # left in the build that does exactly this. There is nothing to compare, and no port
    # can be right or wrong about it.
    for ret_id in re.findall(r'\breturn\s+(\w+)\s*;', blank_rest):
        if re.search(r'\b(?:long|int|short|char|unsigned|signed)[\w \t]*\b%s\s*;'
                     % re.escape(ret_id), blank_rest) and \
                not re.search(r'\b%s\b\s*(?:=[^=]|\+\+|--)' % re.escape(ret_id),
                              blank_rest):
            return None, 'returns the uninitialised local %s: undefined, and the ' \
                         'shipped value is whatever the registers held' % ret_id

    ported = strip_quals(body_text).strip()
    sig = '%s %s%s(%s)' % (ret, (conv[0] + ' ') if conv else '', name,
                           ', '.join('%s %s' % p for p in params) or 'void')
    note = ['/* %s -- shipped at %s.' % (name, shipped.group(1)), ' *',
            ' * Ported by tools/port.py from %s.'
            % os.path.relpath(path, SRC_REPO), ' *']
    if dropped:
        note.append(' * Dropped, because col/globals.h declares them and the addresses'
                    ' agree:')
        for line, names in dropped:
            note.append(' *   %-52s %s' % (line, ', '.join(
                GLOBALS[n]['addr'] for n in names if n in GLOBALS)))
        note.append(' *')
    note += [' * far/near dropped throughout: an addressing mode with no semantic here.',
             ' */']
    src = '\n'.join(note) + '\n#include "col/globals.h"\n\n' + ported + '\n'

    args = ', '.join('(%s)a%d' % (p[0], i) for i, p in enumerate(params))
    case = {'what': 'ported by tools/port.py; compared against the shipped bytes',
            'symbol': name, 'shipped': shipped.group(1),
            'source': 'src/%s/%s.c' % (os.path.basename(os.path.dirname(path)), name),
            'prototype': sig + ';',
            'state': 'shipped', 'ret_bits': INTEGRAL[ret],
            'sweep': sweeps, 'call': ('(long)%s(%s)' if INTEGRAL[ret] else '%s(%s)') % (name, args),
            'globals_read': sorted({n for _, names in dropped for n in names})}
    if calls is not None and not sweeps:
        case['call_args'] = ['0']
        del case['sweep']
    return src, case


def write(path, force=False):
    name = os.path.splitext(os.path.basename(path))[0]
    src, case = port(path)
    if src is None:
        return False, case
    sub = os.path.basename(os.path.dirname(path))
    os.makedirs(os.path.join(REPO, 'src', sub), exist_ok=True)
    dst = os.path.join(REPO, 'src', sub, name + '.c')
    if os.path.exists(dst) and not force:
        return False, 'already ported'
    open(dst, 'w').write(src)
    json.dump(case, open(os.path.join(REPO, 'tests', 'cases', name + '.json'), 'w'),
              indent=1)
    return True, dst


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('paths', nargs='*')
    ap.add_argument('--survey', action='store_true')
    ap.add_argument('--batch', type=int)
    ap.add_argument('--force', action='store_true')
    ap.add_argument('--prune', action='store_true',
                    help='drop ports this tool would no longer make, and their cases')
    a = ap.parse_args()
    corpus = sorted(glob.glob(os.path.join(SRC_REPO, 'matched', '*', '*.c')))

    if a.survey:
        import collections
        why = collections.Counter()
        ok = []
        for p in corpus:
            src, reason = port(p)
            if src is None:
                why[re.sub(r'\b[A-Za-z_]\w*\b(?= )', lambda m: m.group(0), reason)
                    .split(':')[0]] += 1
            else:
                ok.append(p)
        print('%d of %d can be ported as they stand' % (len(ok), len(corpus)))
        for reason, n in why.most_common(12):
            print('  %5d  %s' % (n, reason))
        return 0

    if a.prune:
        gone = 0
        for p in corpus:
            name = os.path.splitext(os.path.basename(p))[0]
            here = glob.glob(os.path.join(REPO, 'src', '*', name + '.c'))
            if not here:
                continue
            if 'Ported by tools/port.py' not in open(here[0]).read():
                continue          # a hand-written port; this tool did not make it
            src, reason = port(p)
            if src is None:
                for f in here + [os.path.join(REPO, 'tests', 'cases', name + '.json')]:
                    if os.path.exists(f):
                        os.remove(f)
                print('%-28s dropped: %s' % (name, reason))
                gone += 1
        print('%d dropped' % gone)
        return 0

    paths = a.paths
    if a.batch:
        paths = []
        for p in corpus:
            name = os.path.splitext(os.path.basename(p))[0]
            if glob.glob(os.path.join(REPO, 'src', '*', name + '.c')):
                continue
            if port(p)[0] is not None:
                paths.append(p)
            if len(paths) >= a.batch:
                break
    done = 0
    for p in paths:
        ok, info = write(p, a.force)
        print('%-28s %s' % (os.path.basename(p),
                            'ported' if ok else 'declined: ' + info))
        done += ok
    print('%d ported' % done)
    return 0


if __name__ == '__main__':
    sys.exit(main())
