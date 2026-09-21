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
WITHDRAWN = json.load(open(os.path.join(REPO, 'model', 'withdrawn.json')))['withdrawn']
# A port that disagrees with the shipped bytes is not a port. Held out by name, with what
# was measured, until someone reads the instructions and says which side is wrong.
DISPUTED = json.load(open(os.path.join(REPO, 'model', 'disputed.json')))['disputed']
MODEL = json.load(open(os.path.join(REPO, 'model', 'model.json')))
RECORDS = MODEL['records']
# What the generated header actually calls each member: the generator renames a member
# whose name the merge gave to two offsets. Rebuilt here from the same rule rather than
# parsed out of the header, so the two cannot drift.
CANON = {}
for _rn, _r in RECORDS.items():
    _used, _by_off = {}, {}
    for _m in sorted(_r['members'], key=lambda m: m['offset']):
        _nm = _m['name']
        if _nm in _used:
            _nm = '%s_%x' % (_nm, _m['offset'])
        _used[_nm] = _m['offset']
        _by_off.setdefault(_m['offset'], (_nm, _m['type']))
    CANON[_rn] = _by_off

MEMBER = re.compile(r'(unsigned char|signed char|unsigned short|unsigned long'
                    r'|unsigned int|unsigned|char|short|int|long)\s+([^;]+);')
WIDTH = {'char': 1, 'signed char': 1, 'unsigned char': 1, 'short': 2,
         'unsigned short': 2, 'int': 2, 'unsigned int': 2, 'unsigned': 2,
         'long': 4, 'unsigned long': 4}
PAD = re.compile(r'^(pad|rest|unused|reserved)\w*$')


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



def dim(expr):
    """`0x75e - 0x149` -> 1557. The reconstructions write a dimension as the arithmetic
    that produced it, which is more legible than the total and has to be read back."""
    expr = expr.strip()
    if not expr or not re.fullmatch(r'[0-9a-fA-FxX +\-*()]+', expr):
        return None
    try:
        return int(eval(expr, {'__builtins__': {}}, {}))      # noqa: S307
    except Exception:
        return None


def width_of(base, pointer):
    if pointer:
        return 4                                  # every pointer here is far
    if base in WIDTH:
        return WIDTH[base]
    if base in RECORDS:
        return RECORDS[base]['size']
    return {'RECT': 8, 'POINT': 4}.get(base)


def local_layout(body):
    """[(offset, name, type, width, is_array)] for a file's own struct, or None.

    The same accumulation the model used, and deliberately as strict: a member this
    cannot read makes every offset after it wrong, and a wrong offset downstream is
    indistinguishable from a right one. So anything unreadable returns None rather than
    a guess.
    """
    out, at = [], 0
    for decl in body.split(';'):
        decl = ' '.join(decl.split())
        if not decl:
            continue
        words = [w for w in re.split(r'\s+', decl) if w not in QUALS]
        if not words:
            return None
        # the base type is the leading type words; the rest are declarators
        base, rest_i = '', 0
        for i, w in enumerate(words):
            cand = (base + ' ' + w).strip()
            if cand in WIDTH or cand in RECORDS or cand in ('RECT', 'POINT') or \
                    w in ('unsigned', 'signed', 'struct', 'void'):
                base, rest_i = cand, i + 1
            else:
                break
        if not base:
            return None
        for part in ' '.join(words[rest_i:]).split(','):
            part = part.strip()
            m = re.match(r'^(\**)\s*(\w+)((?:\s*\[[^\]]*\])*)$', part)
            if not m:
                return None
            pointer, name = bool(m.group(1)), m.group(2)
            dims = re.findall(r'\[([^\]]*)\]', m.group(3))
            unit = width_of(base, pointer)
            if unit is None:
                return None
            n = 1
            for d in dims:
                v = dim(d)
                if v is None:
                    return None
                n *= v
            out.append((at, name, base, unit * n, bool(dims), pointer))
            at += unit * n
    return out


STUB_BODY = re.compile(r'\A(?:return\s+[-\w]+\s*;)?\Z')


def definitions(blank):
    """[(name, body, start, end)] for every function defined in `blank`, brace-matched."""
    out = []
    for m in DEF.finditer(blank):
        i, depth = m.end() - 1, 0
        while i < len(blank):
            if blank[i] == '{':
                depth += 1
            elif blank[i] == '}':
                depth -= 1
                if depth == 0:
                    break
            i += 1
        out.append((m.group('name'), ' '.join(blank[m.end():i].split()),
                    m.start(), i + 1))
    return out


# Borland's int is 16 bits; a host compiler's is 32. A ported body that still says `int`
# is a DIFFERENT PROGRAM on the host -- lfsr_step shifts a word right and the sign bit
# lands in a different place -- so every multi-byte type is respelt through col/types.h,
# where it is `int` under Borland and `short` on the host. char stays char: one byte
# either way, and signed under both toolchains as configured.
RETYPE = [('unsigned char', 'unsigned char'), ('signed char', 'signed char'),
          ('unsigned long', 'col_u32'), ('unsigned short', 'col_u16'),
          ('unsigned int', 'col_u16'), ('unsigned', 'col_u16'),
          ('signed int', 'col_i16'), ('long', 'col_i32'), ('short', 'col_i16'),
          ('int', 'col_i16')]
RETYPE_RE = re.compile(r'\b(%s)\b' % '|'.join(re.escape(k).replace(r'\ ', r'\s+')
                                               for k, _ in RETYPE))
RETYPE_MAP = dict(RETYPE)


def retype(text):
    """Respell the widths, outside comments and strings."""
    mask = blank_comments(text)
    out, at = [], 0
    for m in RETYPE_RE.finditer(mask):
        want = RETYPE_MAP[' '.join(m.group(1).split())]
        out.append(text[at:m.start()])
        out.append(want)
        at = m.end()
    return ''.join(out) + text[at:]


# The disassembly labels the C runtime's own routines in upper case, and a reconstruction
# declares them as ordinary far functions because it cannot include <string.h>. They are
# not reconstructions to port: they come out of CWL.LIB at the link, and the host has them
# too. Only the ANSI ones are here -- Borland's own extensions (strupr, itoa) would not
# link on a host, so a file using those still waits.
# The 180 Windows entry points the shipped program imports, by API name. A call to one of
# these is not a missing port: IMPORT.LIB resolves it at the link, and the emulator
# simulates it. The name comes from the shipped import table, so this cannot drift from
# what the program actually imports.
def _imports():
    out = {}
    for key, nm in json.load(open(os.path.join(SRC_REPO, 're', 'symbols.json')
                                  ))['imports'].items():
        mod, _, ordinal = key.partition('_')
        api = nm[len(mod) + 1:] if nm.startswith(mod + '_') else nm
        out[api.upper()] = (mod, int(ordinal))
    return out


WINAPI = _imports()
TAINT = os.path.join(REPO, 'model', 'api_taint.json')


def tainted():
    try:
        return set(json.load(open(TAINT))['reaches_a_windows_api'])
    except Exception:
        return set()


RUNTIME = {'STRCAT': ('strcat', 'string.h'), 'STRCPY': ('strcpy', 'string.h'),
           'STRNCPY': ('strncpy', 'string.h'), 'STRLEN': ('strlen', 'string.h'),
           'STRCMP': ('strcmp', 'string.h'), 'STRNCMP': ('strncmp', 'string.h'),
           'STRCHR': ('strchr', 'string.h'), 'STRRCHR': ('strrchr', 'string.h'),
           'MEMCPY': ('memcpy', 'string.h'), 'MEMSET': ('memset', 'string.h'),
           'MEMMOVE': ('memmove', 'string.h'), 'MEMCMP': ('memcmp', 'string.h'),
           'TOUPPER': ('toupper', 'ctype.h'), 'TOLOWER': ('tolower', 'ctype.h'),
           'ATOI': ('atoi', 'stdlib.h'), 'ABS': ('abs', 'stdlib.h'),
           'LABS': ('labs', 'stdlib.h')}


def _indexed(text, i):
    """Is the access at `i` immediately subscripted? `c->name[k]` wants a pointer;
    `c->pop` wants the value."""
    while i < len(text) and text[i] in ' \t':
        i += 1
    return i < len(text) and text[i] == '['


def receiver_before(text, i):
    """The primary expression that ends at `i`, or None.

    Walking backwards beats a regex here: a receiver can be `g_colonies[unit_colony(u)]`
    or `((Colony far *)p)`, and balancing brackets is the only way to find where it
    starts. Returns the slice, so the caller can rebuild the access around it.
    """
    j = i
    while j > 0 and text[j - 1] in ' \t':
        j -= 1
    end = j
    depth = 0
    while j > 0:
        c = text[j - 1]
        if c in ')]':
            depth += 1
        elif c in '([':
            if depth == 0:
                break
            depth -= 1
        elif depth == 0 and not (c.isalnum() or c in '_.' or
                                 (c == '>' and text[j - 2:j] == '->') or
                                 (c == '-' and text[j:j + 1] == '>')):
            break
        j -= 1
    expr = text[j:end].strip()
    return expr or None


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
    """Drop far/near/huge, and tidy only the gap each one left.

    Collapsing ALL runs of spaces also eats the indentation, which turned every ported
    body into a flat one-space-indented block. The reconstructions are readable and the
    ports should stay that way.
    """
    for q in QUALS:
        text = re.sub(r'\b%s\b' % q, ' ', text)
    out = []
    for line in text.split('\n'):
        lead = line[:len(line) - len(line.lstrip())]
        out.append(lead + re.sub(r'[ \t]+', ' ', line.strip()))
    return '\n'.join(out)


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
        if '[' in part:
            return None
        words = part.replace('*', ' * ').split()
        stars = words.count('*')
        words = [w for w in words if w != '*']
        if len(words) < 2:
            return None
        name, ty = words[-1], ' '.join(words[:-1])
        if stars > 1:
            return None
        if stars:
            # A pointer parameter: not swept, pointed at a staged region. Its type has to
            # be one this can spell and size, so the region can be big enough.
            if not (ty in WIDTH or ty in RECORDS or ty in ('RECT', 'POINT', 'void')):
                return None
            out.append(('%s *' % ty, name))
            continue
        if ty not in INTEGRAL or INTEGRAL[ty] == 0:
            return None
        out.append((ty, name))
    return out


def index_bounds(rest, params, names):
    """How far argument 0 may be swept before it indexes past a table, or None.

    A sweep that runs off the end of a global array is a harness bug rather than a port
    bug, and it does not look like one: on the host the write lands in whatever the linker
    put next, while the emulator's selector simply ends, so the two disagree and the
    disagreement says nothing about the port. The model knows each table's extent, so ask
    it. Only the UPPER end needs bounding -- a negative index still lands inside the
    segment image, where both sides read the same bytes and agree.
    """
    out = {}
    for pname in [p[1] for p in params if not p[0].endswith('*')]:
        b = _bound_for(rest, pname, names)
        if b is not None:
            out[pname] = b
    return out


def _bound_for(rest, p0, names):
    """(lowest, highest) index that stays inside the segment, or None.

    A segment has two ends. Indexing before the start wraps to the top of the segment in
    16 bits and runs off the front of the array on a host, which is the one difference a
    host cannot reproduce -- set_name_slot with i=-4 lands at SEG31:0xff64 in the game and
    before the array here.
    """
    best = None
    for n in names:
        g = GLOBALS.get(n)
        if not g or not g['dims'] or not re.search(r'\b%s\s*\[\s*%s\b'
                                                   % (re.escape(n), re.escape(p0)), rest):
            continue
        rec = DERIVED.get('records', {})
        # A RECORD-typed table needs its record's size here, not nothing: without it
        # g_nations was left unbounded and the sweep indexed past the end of segment 20,
        # where the host reads the next object and the game wraps.
        width = WIDTH.get(g['base'])
        if width is None and g['base'] in RECORDS:
            width = RECORDS[g['base']]['size']
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
        hi = (SEGSIZE[g['seg']] - g['off']) // width - 1
        lo = -(g['off'] // width)
        best = (lo, hi) if best is None else (max(best[0], lo), min(best[1], hi))
    return best


def scratch(size):
    """A (segment, offset) with `size` bytes no global names, for a pointer argument.

    A pointer parameter cannot be swept -- there is no integer to enumerate -- so the
    case points it at a staged region instead. The region is chosen where the model names
    nothing, so the call perturbs no global the function might also read. Even if it did,
    both sides would see the same bytes, because both address the same segment image;
    this is about keeping the observation legible, not about correctness.
    """
    named = {}
    for g in GLOBALS.values():
        named.setdefault(g['seg'], []).append((g['off'], g['off'] + (g['size'] or 1)))
    for seg in sorted(SEGSIZE, reverse=True):
        spans = sorted(named.get(seg, []))
        at = 0
        for lo, hi in spans + [(SEGSIZE[seg], SEGSIZE[seg])]:
            if lo - at >= size:
                return seg, at
            at = max(at, hi)
    return None


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
    # A file-local struct is not automatically something to unify away. It is one of two
    # things, and only the first is a conflict with the shared header:
    #
    #   it types a GLOBAL   -- `extern Unit far g_units[];` after `typedef struct {...}
    #                          Unit;`. The header already declares that object with the
    #                          MERGED record, so the file's own view of it has to be
    #                          mapped onto that one, member by member, by offset. Not
    #                          done yet: a wrongly named receiver compiles identically,
    #                          so it needs its own checking.
    #
    #   it types a PARAMETER or a local -- `scroll_get_range(Ctl far *c, ...)`. Nothing
    #                          shared is involved: there is no object in the header for
    #                          it to disagree with, and unifying it would be inventing a
    #                          claim rather than porting one. It stays.
    local_types = [m.group(2) for m in
                   re.finditer(r'typedef\s+struct\s*\{(.*?)\}\s*(\w+)\s*;',
                               blank, re.S)]
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
    cut, unknown, file_shape, file_dims, api_protos = [], set(), {}, {}, []
    for start, end in spans:
        line = body[start:end]
        names = re.findall(r'\b(g_\w+)\b', blank[start:end])
        if not names:
            # An extern FUNCTION. A C runtime routine's prototype comes out -- the real
            # header declares it, and the file's own spelling (far pointers, upper case)
            # would clash. Anything else stays: this repo has no header of ported
            # prototypes yet, and the call check refuses the file unless the callee is
            # ported.
            if any(re.search(r'\b%s\b' % r, blank[start:end]) for r in RUNTIME):
                cut.append((start, end))
            elif any(re.search(r'\b%s\b' % a, blank[start:end], re.I)
                     for a in WINAPI):
                # A Windows API prototype. windows.h declares it when Borland builds this,
                # and a second declaration is a type mismatch there; the host has no
                # windows.h and still has to COMPILE the unit, so the file's own
                # declaration is kept for that build only.
                api_protos.append(re.split(r'/\*', line.strip())[0].strip())
                cut.append((start, end))
            continue
        for n in names:
            if n not in GLOBALS and n not in ALIASES:
                # Not necessarily a problem: `g_stub_a` is a global the byte-exact repo
                # invented so that an in-file stub would have a body, and those stubs come
                # out below. Held until the file is what it will be, then re-checked.
                unknown.add(n)
                continue
            want = ADDR.search(line)
            if want and n in GLOBALS:
                a, b = normalise_addr(want.group(1)), GLOBALS[n]['addr']
                if a != b:
                    return None, '%s: the file says %s, the model says %s' % (n, a, b)
        # Keep only the declaration: the trailing /* address */ would close the comment
        # this line is about to be quoted inside, and the model's address is authoritative
        # anyway.
        decl_txt = re.split(r'/\*', line.strip())[0].strip()
        for n in names:
            file_shape[n] = ('array' if re.search(r'%s\s*\[' % re.escape(n), decl_txt)
                             else 'pointer' if re.search(r'\*\s*%s\b' % re.escape(n),
                                                         decl_txt)
                             else 'scalar')
            file_dims[n] = len(re.findall(r'\[', decl_txt.split(n, 1)[-1]))
        dropped.append((decl_txt, names))
        cut.append((start, end))

    rest, blank_rest, at = '', '', 0
    for start, end in cut:
        rest += body[at:start]
        blank_rest += blank[at:start]
        at = end
    rest, blank_rest = rest + body[at:], blank_rest + blank[at:]
    # Two spellings of the same thing: `typedef struct {...} Unit;` and
    # `struct unit {...};`. The second is not a typedef and its tag is lower case, so the
    # record it stands for is found through the GLOBAL declared with it -- which is
    # evidence rather than a name match, and works for both.
    typedefs = {m.group(2): m.group(1) for m in
                re.finditer(r'typedef\s+struct\s*\{(.*?)\}\s*(\w+)\s*;', body, re.S)}
    tags = {m.group(1): m.group(2) for m in
            re.finditer(r'struct\s+(\w+)\s*\{(.*?)\}\s*;', body, re.S)}
    canon_of = {}
    for t in typedefs:
        if t in RECORDS:
            canon_of[t] = t
    for tag in tags:
        for decl_txt, names in dropped:
            if re.search(r'\bstruct\s+%s\b' % re.escape(tag), decl_txt):
                for n in names:
                    base = GLOBALS.get(n, {}).get('base')
                    if base in RECORDS:
                        canon_of['struct ' + tag] = base
                        typedefs['struct ' + tag] = tags[tag]
    # Windows' own structs bring member names of their own into the file; a rewrite of
    # `.right` meant for the game's record would hit `RECT.right` too.
    # Only the members of a Windows struct the file ACTUALLY uses. `x`, `y` and `hwnd`
    # are ordinary game field names too, and guarding them unconditionally refused 202
    # files over a collision that was not there.
    PLATFORM_FIELDS = {'RECT': {'left', 'top', 'right', 'bottom'},
                       'POINT': {'x', 'y'},
                       'MSG': {'hwnd', 'message', 'wParam', 'lParam', 'time', 'pt'}}
    PLATFORM_MEMBERS = set()
    for _t, _f in PLATFORM_FIELDS.items():
        if re.search(r'\b%s\b' % _t, blank):
            PLATFORM_MEMBERS |= _f
    # Every record in play in this file: the ones its own structs stand for, and the ones
    # its globals are declared with. A member name that means a DIFFERENT OFFSET in two of
    # them cannot be rewritten by name -- colonist_specialty_set uses `spec` for both
    # Colony+0x40 and Unit+0x17, and rewriting by name put the colony's offset on a unit.
    # A wrongly named receiver compiles identically, so this is refused rather than
    # guessed at.
    renames, casts, drop_typedefs, protos, near_callees = {}, {}, [], [], []
    host_blind = None
    for t, tbody in typedefs.items():
        if t not in canon_of:
            continue          # types nothing shared: it stays, see above
        lay = local_layout(blank_comments(tbody))
        if lay is None:
            return None, 'its own %s has a member this cannot lay out' % t
        canon = CANON[canon_of[t]]
        used = set(re.findall(r'(?:->|\.)\s*(\w+)', blank))
        for off, mname, ty, width, is_array, is_pointer in lay:
            if mname not in used or PAD.match(mname):
                continue
            if mname in PLATFORM_MEMBERS:
                return None, '%s.%s collides with a Windows struct member of the same ' \
                             'name' % (t, mname)
            # Three outcomes, in order of how much they keep:
            #   the merged record names this byte at this width -> rename, and the port
            #     reads like the record;
            #   it names it at another width, or does not name it, or the file reads it
            #     as an array -> a CAST AT THE SITE, which is what the brief prescribes
            #     for a narrow reading and works equally for an unnamed offset;
            #   the type is one this cannot spell -> refuse.
            cname, ctype = canon.get(off, (None, None))
            if not is_array and cname and WIDTH.get(ctype) == width:
                if cname != mname:
                    renames[mname] = cname
                continue
            if is_pointer:
                # Same as a pointer-valued global: the host cannot hold one at the shipped
                # offset, the 16-bit harness can. Cast at the site and let that oracle
                # own it.
                host_blind = host_blind or ('uses the pointer member %s.%s: four bytes '
                                            'in the game, eight on a host' % (t, mname))
                # Cast it as a POINTER, not as a 32-bit integer: the site returns it,
                # passes it on or dereferences it, and an integer there is a type error
                # rather than a narrower reading.
                casts[mname] = (off, 'void far *', False)
                continue
            elem = ty if (ty in WIDTH or ty in RECORDS or ty in ('RECT', 'POINT')) \
                else None
            if elem is None:
                return None, '%s.%s is a %s at +0x%02x, which needs a cast this cannot ' \
                             'spell' % (t, mname, ty, off)
            casts[mname] = (off, elem, is_array)
        drop_typedefs.append(t)
    in_play = {canon_of[t] for t in drop_typedefs}
    for _, gnames in dropped:
        for n in gnames:
            b = GLOBALS.get(n, {}).get('base')
            if b in RECORDS:
                in_play.add(b)
    used_names = set(re.findall(r'(?:->|\.)\s*(\w+)', blank_rest))
    for rn in sorted(in_play):
        for m2 in RECORDS[rn]['members']:
            if m2['name'] not in used_names:
                continue
            offs = {r2: o2 for r2 in in_play
                    for o2, (nm2, _t2) in CANON[r2].items() if nm2 == m2['name']}
            if len(set(offs.values())) > 1:
                return None, 'the member name %s means %s in this file, and a rewrite ' \
                             'by name cannot tell which receiver is which' \
                             % (m2['name'],
                                ' and '.join('%s+0x%02x' % (r2, o2)
                                             for r2, o2 in sorted(offs.items())))

    if name in WITHDRAWN:
        return None, 'withdrawn: %s' % WITHDRAWN[name]
    if name in DISPUTED:
        return None, 'disputed: %s' % DISPUTED[name]
    shipped = re.search(r'candidate for (\w{4}:\w{4})', head)
    if not shipped:
        return None, 'no shipped address in the header comment'

    # Rule 47: the shipped code reaches a callee in the SAME translation unit with a
    # four-byte PUSH CS / CALL NEAR, and a byte-exact file reproduces that by DEFINING
    # the callee locally -- empty, or `return 0;`. Those definitions are an artefact of
    # byte-matching and every one of them has to go; the real function is ported from its
    # own file and the linker joins them. A file with a second REAL body is a different
    # thing -- two functions that shared a translation unit -- and needs the partition,
    # which is not done.
    defs = definitions(blank_rest)
    if not defs:
        return None, 'no function definition found'
    if name not in [d[0] for d in defs]:
        return None, 'defines %s, not %s' % (defs[0][0], name)
    extras = [d for d in defs if d[0] != name]
    # A second REAL body is a function that shared a translation unit with this one. It
    # does not have to stay here: 412 of the 432 such files have every extra available as
    # its own reconstruction, so the extra comes out like a stub, the call becomes a far
    # call to the separately ported definition, and the linker joins them. That changes
    # the CALL FORM, which mattered for byte-exactness and does not matter here.
    real = [d for d in extras if not STUB_BODY.match(d[1])]
    orphan = [d[0] for d in real
              if not glob.glob(os.path.join(SRC_REPO, 'matched', '*', d[0] + '.c'))]
    if orphan:
        return None, 'defines %s with a real body and there is no reconstruction of it ' \
                     'to port separately' % orphan[0]
    if extras:
        # Each dropped stub was also this file's only DECLARATION of that callee, so a
        # prototype has to replace it or the call compiles as implicit-int and the two
        # sides disagree about the return width.
        near_callees.extend(d[0] for d in extras)
        for _n, _b, st, en in extras:
            m2 = DEF.search(blank_rest[st:en])
            if m2:
                protos.append(retype(strip_quals(
                    '%s%s(%s)' % (m2.group('head'), m2.group('name'),
                                  m2.group('args'))).strip()) + ';')
        keep, at = '', 0
        kb = ''
        for _n, _b, st, en in sorted(extras, key=lambda d: d[2]):
            keep += rest[at:st]
            kb += blank_rest[at:st]
            at = en
        rest, blank_rest = keep + rest[at:], kb + blank_rest[at:]
    d = DEF.search(blank_rest)
    if not d or d.group('name') != name:
        return None, 'the subject definition went missing when the stubs came out'
    still = sorted(n for n in unknown if re.search(r'\b%s\b' % n, blank_rest))
    if still:
        return None, 'the model does not know the global %s' % still[0]

    # A callee this repo has not ported yet would not link, and porting it blind is how a
    # port acquires a function nobody checked. Decline, and the file becomes portable the
    # moment its callee is.
    KEYWORDS = {'if', 'while', 'for', 'switch', 'return', 'sizeof', 'do', 'else',
                'void', 'char', 'int', 'short', 'long', 'unsigned', 'signed', 'float',
                'double', 'struct', 'union', 'const', 'volatile', 'far', 'near', 'huge'}
    called = {c for c in re.findall(r'\b([A-Za-z_]\w*)\s*\(', blank_rest)
              if c not in KEYWORDS and c != name}
    runtime = sorted(c for c in called if c in RUNTIME)
    api = sorted(c for c in called if c.upper() in WINAPI and c not in RUNTIME)
    taint = tainted()
    missing = sorted(c for c in called
                     if c not in RUNTIME and c.upper() not in WINAPI
                     and not glob.glob(os.path.join(REPO, 'src', '*', c + '.c')))
    if missing:
        return None, 'calls %s, which is not ported' % ', '.join(missing[:3])

    params = parse_params(d.group('args'))

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
                # Not a refusal any more. A far pointer is four bytes in the game and
                # eight on a host, so the HOST harness cannot judge such a function -- but
                # tests/emu_differential.py compiles with Borland and runs in the
                # emulator, where four bytes is what a far pointer is. Port it, and say
                # which oracle owns it.
                host_blind = host_blind or ('uses the pointer global %s: four bytes in '
                                            'the game, eight on a host' % n)

    if re.search(r'\(\s*\*\s*g_\w+\s*\)\s*\(', blank_rest):
        return None, 'calls through a global function pointer, which the model types as ' \
                     'data'

    # Lay the arguments out as the emulator sees them: 16-bit WORDS. A scalar is one
    # word and gets a swept axis; a pointer is two (offset, then selector) and gets a
    # fixed pair, because there is no integer to enumerate -- it points at a staged
    # region instead, and the host is handed the same address in its own coordinates.
    npoint = sum(1 for t, _ in params if t.endswith('*'))
    nscalar = len(params) - npoint
    if nscalar > 3 or len(params) > 5:
        return None, '%d arguments is more than a sweep should enumerate' % len(params)
    # A negative argument is worth sweeping -- it is where signedness shows -- but not
    # into a table. shift_table_down(-4) walks a loop from 14 down to -4 and indexes the
    # global at every step: in 16 bits that wraps inside the segment, and on a host it
    # runs off the front of the array. The index need not be the parameter itself, so the
    # only safe rule is: a function that indexes any global at all is swept from 0.
    lowest = 0 if re.search(r'\bg_\w+\s*\[', blank_rest) else -4
    per = {0: [], 1: ['0=%d..24' % lowest], 2: ['0=0..7', '1=0..7'],
           3: ['0=0..3', '1=0..3', '2=0..3']}[nscalar]
    base = scratch(64 * max(1, npoint) + 512) if npoint else None
    if npoint and base is None:
        return None, 'no room in any data segment to stage %d pointer argument%s' \
            % (npoint, '' if npoint == 1 else 's')
    bounds = index_bounds(rest, params, [n for _, ns in dropped for n in ns])
    sweeps, callargs, stage, k, blk, si = [], [], {}, 0, 0, 0
    for ty, pname in params:
        if ty.endswith('*'):
            pt = ty[:-1].strip()
            off = base[1] + blk * 64
            blk += 1
            sel = 0x1000 + (base[0] - 1) * 8
            sweeps += ['%d=%d' % (k, off), '%d=%d' % (k + 1, sel)]
            callargs.append('(%s far *)(col_seg%d + 0x%04xL)'
                            % (retype(pt), base[0], off))
            # Something to read that is not all zeroes, identical on both sides, and
            # DIFFERENT PER ARGUMENT: two pointers staged with the same bytes made
            # swap_ints a no-op, so a port that did nothing passed its own selftest.
            stage['SEG%d:0x%04x' % (base[0], off)] = \
                ''.join('%02x' % ((i * 7 + 3 + (blk - 1) * 29) & 0xFF)
                        for i in range(64))
            k += 2
        elif ty.endswith('long'):
            # A long is TWO words on the stack, low first. Sweeping it as one would put
            # every later argument on the wrong slot; sweeping the low word and fixing
            # the high one at 0 keeps the layout right and still varies the value.
            rng = per[si].split('=')[1]
            if pname in bounds and '..' in rng:
                lo, _, hi = rng.partition('..')
                b = bounds[pname]
                rng = '%d..%d' % (max(int(lo), b[0]), min(int(hi), b[1]))
            sweeps += ['%d=%s' % (k, rng), '%d=0' % (k + 1)]
            # Build the long from BOTH words, the way the stack holds it. Passing just the
            # low word sign-extended it on the host while the emulator saw a positive
            # value with a zero high word, so every negative argument disagreed.
            callargs.append('(%s)(((col_u32)(col_u16)a%d << 16) | (col_u16)a%d)'
                            % (retype(ty), k + 1, k))
            k += 2
            si += 1
        else:
            rng = per[si].split('=')[1]
            # An index that runs off the end of a table is a harness bug, not a port bug:
            # on the host the access lands in whatever is next, while the emulator's
            # selector simply ends. The model knows where each segment stops.
            if pname in bounds and '..' in rng:
                lo, _, hi = rng.partition('..')
                b = bounds[pname]
                rng = '%d..%d' % (max(int(lo), b[0]), min(int(hi), b[1]))
            sweeps.append('%d=%s' % (k, rng))
            callargs.append('(%s)a%d' % (retype(ty), k))
            k += 1
            si += 1

    ret = strip_quals(d.group('head')).strip()
    for c in CONV:
        ret = re.sub(r'\b%s\b' % c, '', ret).strip()
    ret_is_ptr = ret.rstrip().endswith('*')
    if ret_is_ptr:
        # A returned far pointer is four bytes in DX:AX, which the 16-bit harness compares
        # exactly. On a host it is a host pointer and comparing it means nothing, so the
        # host case compares memory only and says so.
        ret = 'void'
    if ret not in INTEGRAL:
        return None, 'returns %r, which the comparison cannot mask' % ret
    void_blind = (INTEGRAL[ret] == 0 and not dropped
                  and not any(t.endswith('*') for t, _ in params))

    # col/globals.h gives an array-shaped global a POINTER form, because a global one file
    # indexes and another reads whole is indexable in the header and scalar at the site.
    # Rewrite the scalar uses; a use that takes its ADDRESS has no such rewrite, so refuse.
    body_pre = rest
    # The header may give a global MORE dimensions than the file used: g_nation_bits is
    # `char (*)[0x13c]` here and `char g_nation_bits[]` there, so the file's
    # `g_nation_bits[row * 0x13c + byte]` would index whole ROWS. Flatten it at the site.
    for _, gnames in dropped:
        for n in gnames:
            g = GLOBALS.get(n)
            if not g or len(g['dims']) <= file_dims.get(n, 0):
                continue
            # Only a ROW STRIDE the file did not use, over a scalar element. A record-typed
            # array is a different thing: flattening `Father g_fathers[]` to `col_i16 *`
            # makes `g_fathers[i].cat` a member reference on a short.
            if g['base'] not in WIDTH or not file_dims.get(n):
                continue
            elem = retype(g['base'])
            body_pre = re.sub(r'(?<![\w.>])%s\b' % re.escape(n),
                              '((%s far *)%s)' % (elem, n), body_pre)
    body_text = body_pre
    for t in drop_typedefs:
        if t.startswith('struct '):
            tag = t.split()[1]
            body_text = re.sub(r'struct\s+%s\s*\{.*?\}\s*;\s*' % re.escape(tag),
                               '', body_text, flags=re.S)
            body_text = re.sub(r'\bstruct\s+%s\b' % re.escape(tag), canon_of[t],
                               body_text)
        else:
            body_text = re.sub(
                r'typedef\s+struct\s*\{.*?\}\s*%s\s*;\s*' % re.escape(t),
                '', body_text, flags=re.S)
    for old, new in renames.items():
        body_text = re.sub(r'(->|\.)(\s*)%s\b' % re.escape(old),
                           lambda m: m.group(1) + m.group(2) + new, body_text)
    for cname_, (off, elem, was_array) in sorted(casts.items()):
        while True:
            m = re.search(r'(->|\.)\s*%s\b' % re.escape(cname_), body_text)
            if not m:
                break
            recv = receiver_before(body_text, m.start())
            if recv is None:
                return None, 'cannot see what %s is a member of' % cname_
            base = '(%s)' % recv if m.group(1) == '->' else '&(%s)' % recv
            rep = ('((%s *)((unsigned char far *)%s + 0x%x))' if elem.endswith('*')
                   else '((%s far *)((unsigned char far *)%s + 0x%x))') % (
                       elem, base, off)
            if m.group(1) == '.' or True:
                pass
            # An ARRAY member is always the pointer form: it decays to one in every
            # context, and `w->msg` passed to strlen is as common as `w->msg[i]`.
            # Dereferencing it gave strlen a char. Only a scalar taken through a cast
            # needs the `*`.
            use = rep if (was_array or _indexed(body_text, m.end())) else '(*%s)' % rep
            body_text = (body_text[:m.start() - len(recv)] + use
                         + body_text[m.end():])
    for _, names in dropped:
        for n in names:
            if not GLOBALS.get(n, {}).get('dims'):
                continue
            if file_shape.get(n) != 'scalar':
                # The file declares it as an array too, so a bare use already means
                # "pointer to the first element" and wrapping it in `(*...)` would pass a
                # char where a char * belongs.
                continue
            # `&g_x` where the header spells the global as a pointer-to-first-element:
            # the two denote the same address, so the & simply comes off. Refusing it cost
            # MSDisposeHandle and everything behind it.
            body_pre = re.sub(r'&\s*%s\b' % re.escape(n), '(%s)' % n, body_pre)
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

    # The reconstruction includes the byte-exact repo's w16types.h for RECT and friends.
    # col/win16.h provides those here -- windows.h on Win16, and 16-bit-wide copies on the
    # host, where w16types.h's bare `int` would put the members at the wrong offsets.
    body_text = re.sub(r'^[ \t]*#\s*include\s+"w16types\.h".*\n?', '', body_text,
                       flags=re.M)
    for r in runtime:
        body_text = re.sub(r'\b%s\b' % r, RUNTIME[r][0], body_text)
    ported = retype(strip_quals(body_text)).strip()
    sig = retype('%s %s%s(%s)'
                 % (strip_quals(d.group('head')).strip()
                    if ret_is_ptr else ret,
                    (conv[0] + ' ') if conv else '', name,
                    ', '.join('%s %s' % p for p in params) or 'void'))
    for c in CONV:
        sig = re.sub(r'\b%s\b' % c, '', sig).strip() if ret_is_ptr and not conv else sig
    note = ['/* %s -- shipped at %s.' % (name, shipped.group(1)), ' *',
            ' * Ported by tools/port.py from %s.'
            % os.path.relpath(path, SRC_REPO), ' *']
    if runtime:
        note.append(' * %s: the C runtime\'s own, taken from the real header rather than'
                    % ', '.join('%s -> %s' % (r, RUNTIME[r][0]) for r in runtime))
        note.append(' * from this file\'s far-pointer prototype.')
        note.append(' *')
    if dropped:
        note.append(' * Dropped, because col/globals.h declares them and the addresses'
                    ' agree:')
        for line, names in dropped:
            note.append(' *   %-52s %s' % (line, ', '.join(
                GLOBALS[n]['addr'] for n in names if n in GLOBALS)))
        note.append(' *')
    if drop_typedefs:
        note.append(' * The file\'s own %s dropped for the merged record in '
                    'col/records.h.' % ', '.join(drop_typedefs))
        if renames:
            note.append(' * Members renamed to the merged record\'s spelling, by OFFSET: '
                        + ', '.join('%s -> %s' % kv for kv in sorted(renames.items())))
        note.append(' *')
    note += [' * far/near dropped throughout: an addressing mode with no semantic here.',
             ' */']
    src = '\n'.join(note) + '\n'
    for h in sorted({RUNTIME[r][1] for r in runtime}):
        src += '#include <%s>\n' % h
    src += '#include "col/globals.h"\n\n'
    if protos:
        src += ('/* The callees whose in-file stub came out: declared here, defined in '
                'their own\n * translation units, joined by the linker. */\n'
                + '\n'.join(sorted(set(protos))) + '\n\n')
    if api_protos:
        src += ('/* Windows APIs. windows.h declares these when Borland builds this; the\n'
                ' * host has no windows.h and still has to compile the unit. */\n'
                '#ifdef COL_HOST\n'
                + '\n'.join(retype(strip_quals(x)).rstrip(';') + ';'
                             for x in sorted(set(api_protos)))
                + '\n#endif\n\n')
    src += ported + '\n'
    bad = compiles(src)
    if bad:
        return None, 'the port does not compile: %s' % bad

    args = ', '.join(callargs)
    case = {'what': 'ported by tools/port.py; compared against the shipped bytes',
            'symbol': name, 'shipped': shipped.group(1),
            'source': 'src/%s/%s.c' % (os.path.basename(os.path.dirname(path)), name),
            'prototype': sig + ';',
            'state': 'shipped', 'ret_bits': INTEGRAL[ret],
            # What the 16-bit harness compares, when it differs: a returned far pointer is
            # a real 32-bit value there and meaningless on a host.
            'ret_bits_emu': 32 if ret_is_ptr else INTEGRAL[ret],
            'sweep': sweeps, 'stage': stage,
            'call': ('(long)%s(%s)' if INTEGRAL[ret] else '(void)%s(%s)'
                     if ret_is_ptr else '%s(%s)') % (name, args),
            'globals_read': sorted({n for _, names in dropped for n in names}),
            # Callees the SHIPPED code reaches with a four-byte PUSH CS / CALL NEAR from
            # inside its own segment (rule 47). tests/emu_differential.py cannot run such
            # a function: it loads the candidate in a selector of its own, so a far call
            # to a near callee returns through the wrong kind of RET.
            'near_callees': sorted(set(near_callees))}
    # A call that reaches a Windows API, directly or through a ported callee, cannot be
    # judged on the host: there is no such API there, and a stub would be a different
    # program. The emulator simulates them, so the 16-bit harness owns these.
    reaches_api = bool(api) or bool(called & taint)
    if reaches_api and not host_blind:
        host_blind = ('calls %s, a Windows API the host does not have; the emulator '
                      'simulates it' % (api[0] if api else sorted(called & taint)[0]))
    if void_blind and not host_blind:
        # Nothing the HOST can see: no return value, no global, no pointer written
        # through. The 16-bit harness still compares the stack delta and every write the
        # call makes, so it owns these rather than nobody owning them.
        host_blind = ('returns void and touches nothing the host can observe; the 16-bit '
                      'harness compares the stack and the writes its call makes')
    if runtime and not host_blind:
        # The host links its OWN strcpy; the shipped code calls Borland's. Same contract,
        # different code, and on a staged buffer that does not terminate inside itself
        # they part company. The 16-bit harness calls the very routine the game calls.
        host_blind = ('calls %s: the host links its own, the shipped code calls '
                      "Borland's" % ', '.join(RUNTIME[r][0] for r in runtime))
    if host_blind:
        case['host_blind'] = host_blind
    if reaches_api:
        case['reaches_windows_api'] = True
    # A forwarder: its whole body is one call to a callee the shipped code reaches with a
    # near call. The host sees no effect of its own, and the 16-bit harness cannot place
    # the candidate in the callee's segment, so NEITHER oracle can check it. Said here, so
    # the suite can report it as unverified rather than as green or as a failure.
    if near_callees and re.fullmatch(
            r'\s*(?:return\s+)?\w+\s*\([^;{}]*\)\s*;\s*',
            definitions(blank_comments(ported))[0][1] if definitions(
                blank_comments(ported)) else 'x'):
        case['unverifiable'] = ('a forwarder to %s, which the shipped code reaches with a '
                                'near call: the host sees no effect of its own and the '
                                '16-bit harness cannot place the candidate in that '
                                'segment' % sorted(set(near_callees))[0])
    if not sweeps:
        case['call_args'] = ['0']
        del case['sweep']
    if not stage:
        del case['stage']
    return src, case


def compiles(text):
    """Does this port compile? Returns None, or the first error.

    A port that does not compile is not a port, and it should never reach src/. The tier-B
    rewrites are where this bites: a cast at the site has to guess an element type, and
    when the guess is wrong the compiler says so -- which is worth catching here rather
    than in the suite, where it looks like a harness failure.
    """
    import subprocess, tempfile
    with tempfile.NamedTemporaryFile('w', suffix='.c', delete=False) as f:
        f.write(text)
        tmp = f.name
    try:
        r = subprocess.run(
            [os.environ.get('CC', 'cc'), '-std=c89', '-DCOL_HOST=1', '-fsigned-char',
             '-fwrapv', '-fno-strict-aliasing', '-Wno-unused-value', '-Werror',
             '-fsyntax-only', '-I' + os.path.join(REPO, 'include'), tmp],
            capture_output=True, text=True)
    finally:
        os.remove(tmp)
    if r.returncode:
        for ln in r.stderr.splitlines():
            if ' error: ' in ln:
                return ln.split(' error: ', 1)[1].strip()[:80]
        return (r.stderr.strip().splitlines() or ['it does not compile'])[0][:80]
    return None


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
    if case.get('reaches_windows_api'):
        t = tainted() | {name}
        json.dump({'what': 'ports whose call graph reaches a Windows API, so the host '
                           'cannot judge them and the 16-bit harness owns them',
                   'reaches_a_windows_api': sorted(t)}, open(TAINT, 'w'), indent=1)
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
