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
# `extern "C"` in front of the definition. The byte-exact repo puts it on every C++ file
# whose function keeps a C name, and the quotes are not in the head's character class, so
# the definition was invisible and 18 files reported having none.
DEF = re.compile(r'^(?:extern\s*"C"\s+)?'
                 r'(?P<head>[A-Za-z_][\w\s\*]*?)(?P<name>\w+)\s*\((?P<args>[^;{]*)\)\s*\{',
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
        # `struct Item far *next` -- a self-referential list node. `struct` alone is not
        # a type, and the tag that follows it names a struct whose layout is this one,
        # still being read. As a POINTER it needs no layout, only its four bytes.
        if base == 'struct' and rest_i < len(words) and \
                re.fullmatch(r'\w+', words[rest_i]) and \
                ' '.join(words[rest_i + 1:]).lstrip().startswith('*'):
            base, rest_i = 'struct ' + words[rest_i], rest_i + 1
        if not base or base == 'struct':
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


# Macros, not functions. windows.h defines all of these; a call to one compiles to
# shifts and masks with nothing to link, so demanding a port of them held up 47 files
# over a callee that does not exist.
WINMACRO = {'MAKELONG', 'MAKELP', 'MAKEINTRESOURCE', 'MAKEINTATOM', 'MAKEPOINT',
            'LOWORD', 'HIWORD', 'LOBYTE', 'HIBYTE', 'SELECTOROF', 'OFFSETOF',
            'FIELDOFFSET', 'RGB', 'GetRValue', 'GetGValue', 'GetBValue',
            'MAKELRESULT', 'MAKEWPARAM', 'MAKELPARAM'}

# Borland's C++ runtime, as the disassembly labels it. `mem_new` is what the byte-exact
# repo calls `operator new(unsigned)`, and the shipped image has it under its mangled
# name. Spelling it the shipped way is what lets the 16-bit harness point the call at
# the very allocator the shipped function calls, instead of at a second heap.
SHIPPED_ALIAS = {'mem_new': '__bnew_qui_rtl', 'mem_free': '__bdele_qnv_rtl'}


def _shipped_labels():
    """Every name the shipped image has a label for, spelled as a call site might."""
    try:
        labels = json.load(open(os.path.join(SRC_REPO, 're', 'symbols.json')))['labels']
    except Exception:
        return set()
    out = set()
    for nm in labels.values():
        out |= {nm, nm.lower(), nm.upper(), nm + '@'}
    return out


SHIPPED = _shipped_labels()


def ported_paths(name):
    """The ports of exactly this name, matched CASE-SENSITIVELY.

    glob on macOS asks a case-insensitive filesystem, so `src/*/ATOL.c` came back with
    `src/game/atol.c` -- a different function -- and ATOL was taken for ported. The case
    was then not marked host-blind, and the host archive would not link for want of
    `_ATOL`, which broke every other case that linked against that unit too.
    """
    # os.listdir, not glob: on a case-insensitive filesystem glob answers with the
    # PATTERN's spelling, so filtering its results by basename compares `ATOL` with
    # `ATOL` and always agrees. Only the directory listing says what is really there.
    out = []
    for d in glob.glob(os.path.join(REPO, 'src', '*')):
        if not os.path.isdir(d):
            continue
        for f in os.listdir(d):
            stem, ext = os.path.splitext(f)
            if stem == name and ext in ('.c', '.cpp'):
                out.append(os.path.join(d, f))
    return out


def _indexed(text, i):
    """Is the access at `i` immediately subscripted? `c->name[k]` wants a pointer;
    `c->pop` wants the value."""
    while i < len(text) and text[i] in ' \t':
        i += 1
    return i < len(text) and text[i] == '['


class Opaque(Exception):
    """A file's opaque class and the model's record disagree about a size."""


class Ambiguous(Exception):
    """A member name that means two offsets, at a receiver this cannot type."""


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


def parse_params(args, local_types=()):
    """[(type, name)] for a parameter list, or None if it has something unsweepable."""
    # `const char far *s` and `register int u` are ordinary parameters: the qualifiers
    # say nothing about how to generate a value for them.
    args = re.sub(r'\b(?:const|volatile|register|auto)\b', ' ', strip_quals(args)).strip()
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
            # A pointer to a type THIS FILE declares is fine: the case stages a region
            # and the file's own declaration says how to read it. 146 functions were
            # refused for a pointee the model does not happen to name.
            if not (ty in WIDTH or ty in RECORDS or ty in local_types
                    or ty in ('RECT', 'POINT', 'void')):
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


# A C++ reconstruction: class declarations with opaque bodies and one method defined out
# of line. Borland compiles it by extension and TDUMP demangles the symbols, so the method
# can be sliced by the name a person would write -- `AboutWin::~AboutWin()`.
CPP_DEF = re.compile(r'^(?P<head>[\w\s\*:~]*?)(?P<cls>\w+)::(?P<meth>~?\w+)\s*'
                     r'\((?P<args>[^;{]*)\)\s*(?:const\s*)?\{', re.M)


def globals_for(blank):
    """The `#define`s for the globals a C++ file uses, and nothing else."""
    used = sorted({n for n in re.findall(r'\b(g_\w+)\b', blank) if n in GLOBALS})
    if not used:
        return ''
    out = ['#include "col/segments.h"', '']
    for n in used:
        g = GLOBALS[n]
        cast = retype(g['base']) + (' far ' + '*' * g['stars'] if g['stars'] else '')
        body = ('((%s far *)(col_seg%d + 0x%04xL))' if g['dims'] else
                '(*(%s far *)(col_seg%d + 0x%04xL))') % (cast.strip(), g['seg'], g['off'])
        out.append('#define %-34s %-44s /* %s */' % (n, body, g['addr']))
    return '\n'.join(out) + '\n\n'


def cpp_port(path, name, head, body, blank):
    """(source, case) for a C++ file, or (None, reason).

    The classes stay as the file declares them. Unifying them the way tier B unifies a
    struct would need a shared class header and the vtable layout to go with it; nothing
    here needs that yet, because what is being compared is one method against the shipped
    bytes of that same method.
    """
    defs = list(CPP_DEF.finditer(blank))
    if not defs:
        return None, 'a C++ file this cannot find a method definition in'
    # The subject is the method the FILE is named for: AboutWin_ctor -> AboutWin::AboutWin,
    # AboutWin_dtor -> AboutWin::~AboutWin. The others are base-class constructors the
    # compiler needed here and that have reconstructions of their own, so they come out and
    # the linker joins them -- the same rule as a C file with a second real body.
    want = None
    m0 = re.match(r'^(\w+?)_(ctor|dtor)(?:_|$)', name)
    if m0:
        want = (m0.group(1), ('~' if m0.group(2) == 'dtor' else '') + m0.group(1))
    subject = None
    for d0 in defs:
        if want and (d0.group('cls'), d0.group('meth')) == want:
            subject = d0
    if subject is None and len(defs) == 1:
        subject = defs[0]
    if subject is None:
        return None, 'a C++ file defining %d methods and none of them the one it is ' \
                     'named for' % len(defs)
    extras = [d0 for d0 in defs if d0 is not subject]
    orphan = [d0 for d0 in extras
              # `GameWin_ctor_1010_28a9.c` as well as `GameWin_ctor.c`: the image has
              # several constructors for some classes and the corpus suffixes them.
              if not glob.glob(os.path.join(SRC_REPO, 'matched', '*', '%s_%s*.c'
                                            % (d0.group('cls'),
                                               'dtor' if d0.group('meth').startswith('~')
                                               else 'ctor')))]
    if orphan:
        return None, 'a C++ file also defining %s::%s, which has no reconstruction of ' \
                     'its own' % (orphan[0].group('cls'), orphan[0].group('meth'))
    for d0 in sorted(extras, key=lambda x: x.start(), reverse=True):
        i, depth = d0.end() - 1, 0
        while i < len(blank):
            if blank[i] == '{':
                depth += 1
            elif blank[i] == '}':
                depth -= 1
                if depth == 0:
                    break
            i += 1
        body = body[:d0.start()] + body[i + 1:]
        blank = blank[:d0.start()] + blank[i + 1:]
    d = subject
    cls, meth = d.group('cls'), d.group('meth')
    if d.group('args').strip() not in ('', 'void'):
        return None, 'a C++ method with arguments, which the case generator cannot sweep '\
                     'yet'
    shipped = re.search(r'candidate for (\w{4}:\w{4})', head)
    if not shipped:
        return None, 'no shipped address in the header comment'
    symbol = '%s::%s()' % (cls, meth)
    # w16types.h is this file's ONLY source of RECT unless col/segments.h is pulled in
    # (which reaches windows.h). Strip it exactly when something else will declare them --
    # stripping it unconditionally broke seven files that needed it.
    uses_globals = bool(re.search(r'\bg_\w+', blank))
    if uses_globals:
        body = re.sub(r'^[ \t]*#\s*include\s+"w16types\.h".*\n?', '', body, flags=re.M)
    ported = retype(strip_quals(body)).strip()
    src = ('/* %s -- shipped at %s.\n *\n * Ported by tools/port.py from %s.\n *\n'
           ' * C++: the classes are as the file declares them, and the method is compiled\n'
           ' * by Borland and sliced from the object by its demangled name. The host\n'
           ' * harness does not judge these -- a constructor\'s work is member\n'
           ' * construction and a vtable, which a host build does not reproduce.\n */\n'
           % (symbol, shipped.group(1), os.path.relpath(path, SRC_REPO))
           # NOT col/globals.h: it carries the merged records, and several share a NAME
           # with a class declared here (EventWin, CTable, GRPort), which is a
           # redeclaration in C++ and broke 180 of these files. The globals this file
           # actually uses are emitted instead, at the addresses the model gives, typed
           # with the model's own type name -- which resolves to the class THIS FILE
           # declares,, which is what the original code meant by it.
           # types.h always: the body is respelt through col_i16 and friends whether or
           # not it touches a global, and it carries no record typedefs to collide with.
           # COL_CPP_PORT keeps col/win16.h's opaque GameWin/Dlg/MrTimer typedefs out of
           # the way of the classes this file declares with those names.
           + '#define COL_CPP_PORT 1\n#include "col/types.h"\n\n'
           + globals_for(blank) + ported + '\n')
    base = scratch(1024)
    if base is None:
        return None, 'no room to stage a `this` pointer'
    sel = 0x1000 + (base[0] - 1) * 8
    case = {'what': 'a C++ method, compared against the shipped bytes in 16 bits',
            # The flags the byte-exact repo determined for THIS function, from its own
            # header. -Od matters: without it Borland's codegen diverges eight bytes in.
            'flags': (re.search(r'flags:\s*([-\w \t]+)', head).group(1).strip()
                      if re.search(r'flags:\s*([-\w \t]+)', head) else
                      '-ml -WS -Od -Ot -3 -P'),
            'symbol': symbol, 'shipped': shipped.group(1),
            'source': 'src/%s/%s.cpp' % (os.path.basename(os.path.dirname(path)), name),
            'prototype': '', 'state': 'shipped', 'ret_bits': 0, 'ret_bits_emu': 16,
            'sweep': ['0=%d' % base[1], '1=%d' % sel],
            'call': '', 'globals_read': [], 'near_callees': [],
            'host_blind': 'a C++ method: the host harness has no way to call it, and a '
                          'constructor\'s work is member construction and a vtable'}
    return src, case


def port(path, force=False):
    """(source text, case dict) or (None, reason)."""
    name = os.path.splitext(os.path.basename(path))[0]
    # FIRST, before any dispatch or parsing. This guard has been lost twice: once behind
    # the C++ dispatch, and once to an edit whose pattern did not match, which DELETED it
    # silently. Everything held out came straight back on the next cascade both times.
    if name in WITHDRAWN:
        return None, 'withdrawn: %s' % WITHDRAWN[name]
    if name in DISPUTED:
        return None, 'disputed: %s' % DISPUTED[name]
    text = open(path, errors='replace').read()
    m = HEADER.match(text)
    head, body = (m.group(0), text[m.end():]) if m else ('', text)

    # `extern "C" {` ... `}` around the whole file. The declarations inside it are cut
    # out one by one, and the brace it opened was then left behind as a stray `}` in the
    # middle of the port. Blanked to spaces, in place, so every offset this tool takes
    # from `body` still means what it meant.
    while True:
        mb = re.search(r'extern\s*"C"\s*\{', blank_comments(body))
        if not mb:
            break
        depth, k = 0, mb.end() - 1
        bl = blank_comments(body)
        while k < len(bl):
            if bl[k] == '{':
                depth += 1
            elif bl[k] == '}':
                depth -= 1
                if depth == 0:
                    break
            k += 1
        if k >= len(bl):
            return None, 'an extern "C" block this cannot find the end of'
        body = (body[:mb.start()] + ' ' * (mb.end() - mb.start())
                + body[mb.end():k] + ' ' + body[k + 1:])

    blank = blank_comments(body)
    # A class declaration alone does not make this the C++ path. ColChild_DragIcon
    # declares `class GRPort` and then defines a plain C function inside `extern "C"`: it
    # needs C++ COMPILATION (the header says -P) and nothing else. Only a file that
    # defines a Class::method goes through cpp_port.
    is_cpp = bool(re.search(r'^\s*class\s+\w+', blank, re.M) or 'extern "C"' in blank)
    if CPP_DEF.search(blank):
        return cpp_port(path, name, head, body, blank)
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
            # And the WIDTH, which the address check used to let through. The header
            # spelt g_game_flags a byte where this file spells it a word; dropping the
            # file's declaration for the header's truncated `^= 0x2000` to `^= 0` and the
            # port flipped nothing, silently and with a clean compile. A NARROWER header
            # is never safe; a wider one is only a read of bytes the file does not use.
            g = GLOBALS.get(n)
            # The header's DIMENSIONS are not the question -- `char g_x[]` against
            # `unsigned g_x` is exactly the case that bit -- only its element width
            # against the one the file declares.
            if g is not None and not g.get('stars'):
                decl = re.split(r'/\*', line.strip())[0]
                base = re.sub(r'\b(?:extern|far|near|const|volatile)\b', ' ',
                              decl.split(n)[0]).strip()
                fw = WIDTH.get(' '.join(base.split()))
                hw = g.get('size')
                if fw and hw and fw > hw and '*' not in decl.split(n)[0] \
                        and '[' not in decl.split(n, 1)[-1]:
                    return None, '%s is %d bytes in this file and %d in the model: the ' \
                                 'header would truncate every value written through it' \
                                 % (n, fw, hw)
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
    # A TAGGED typedef -- `typedef struct Item { ... } Item;` -- is neither of the two
    # forms this used to look for, so the file's own struct was invisible: its members
    # were never mapped onto a record, and its name collided with the header's typedef
    # of the same name. The tag is optional here, and remembered, because dropping the
    # typedef has to drop the tag with it.
    TD = re.compile(r'typedef\s+struct\s*(\w+)?\s*\{(.*?)\}\s*(\w+)\s*;', re.S)
    typedefs = {m.group(3): m.group(2) for m in TD.finditer(body)}
    tag_of = {m.group(3): m.group(1) for m in TD.finditer(body) if m.group(1)}
    tagged = set(tag_of.values())
    tags = {m.group(1): m.group(2) for m in
            re.finditer(r'struct\s+(\w+)\s*\{(.*?)\}\s*;', body, re.S)
            if m.group(1) not in tagged}
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
    # Which record each by-name rule was derived from. A name that means one offset
    # everywhere needs no receiver: the rule is the same whoever it is applied to. A
    # name that means two needs this, to apply the rule only to ITS record's receivers.
    rule_record = {}
    host_blind = None
    for t, tbody in typedefs.items():
        if t not in canon_of:
            continue          # types nothing shared: it stays, see above
        lay = local_layout(blank_comments(tbody))
        if lay is None:
            return None, 'its own %s has a member this cannot lay out' % t
        # If the file SUBSCRIPTS something of this type, its own size is the STRIDE, and
        # the merged record has to agree with it. `Rec` is a placeholder name the model
        # merged across unrelated structs: colonies_swap_words walks 202-byte records and
        # the record called Rec is 1916 bytes, so `r[i]` stepped eight times too far and
        # the port wrote nothing where the shipped code wrote. A name match is not
        # evidence; a matching stride is.
        own = max((o + w for o, _n, _t, w, _a, _p in lay), default=0)
        want = RECORDS[canon_of[t]]['size']
        if own and want and own != want:
            for m3 in re.finditer(r'\b%s\b(?:\s+(?:far|near))*\s*\**\s*(\w+)'
                                  % re.escape(t), blank):
                if re.search(r'\b%s\s*\[' % re.escape(m3.group(1)), blank_rest):
                    return None, 'its own %s is %d bytes and the record of that name is ' \
                                 '%d, and the file indexes an array of them: the stride ' \
                                 'would be wrong' % (t, own, want)
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
                    rule_record[mname] = canon_of[t]
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
                # As a pointer to WHAT the file says it points at, not `void *`.
                # Borland rejects arithmetic on a void pointer -- "Size of the type is
                # unknown or zero" -- where clang takes it as an extension, and
                # MSLockGraf walks `p->rows[y] = p->rows[y-1] - p->width` down a
                # `long far *`. The declared base is both compilable and the file's own
                # reading; `void far *` stays for a pointee this cannot spell.
                pointee = ('%s far *' % ty) if (ty in WIDTH or ty in RECORDS) \
                    else 'void far *'
                casts[mname] = (off, pointee, False)
                rule_record[mname] = canon_of[t]
                continue
            elem = ty if (ty in WIDTH or ty in RECORDS or ty in ('RECT', 'POINT')) \
                else None
            if elem is None:
                return None, '%s.%s is a %s at +0x%02x, which needs a cast this cannot ' \
                             'spell' % (t, mname, ty, off)
            casts[mname] = (off, elem, is_array)
            rule_record[mname] = canon_of[t]
        drop_typedefs.append(t)
    in_play = {canon_of[t] for t in drop_typedefs}
    for _, gnames in dropped:
        for n in gnames:
            b = GLOBALS.get(n, {}).get('base')
            if b in RECORDS:
                in_play.add(b)
    used_names = set(re.findall(r'(?:->|\.)\s*(\w+)', blank_rest))
    ambiguous = {}
    for rn in sorted(in_play):
        for m2 in RECORDS[rn]['members']:
            if m2['name'] not in used_names:
                continue
            # Only a name a REWRITE touches is ambiguous. If the file spells the
            # member the way both records already spell it, nothing is keyed by that
            # name: `p->name` on a Colony is Colony+0x02 and on a UnitType is +0x00,
            # and the C compiler picks by the receiver's type exactly as the shipped
            # code does. Refusing those cost 29 files over a collision that only
            # exists for names this actually substitutes.
            if m2['name'] not in renames and m2['name'] not in casts:
                continue
            offs = {r2: o2 for r2 in in_play
                    for o2, (nm2, _t2) in CANON[r2].items() if nm2 == m2['name']}
            if len(set(offs.values())) > 1:
                if m2['name'] not in rule_record:
                    return None, 'the member name %s means %s in this file, and the ' \
                                 'rule for it belongs to no record' \
                                 % (m2['name'],
                                    ' and '.join('%s+0x%02x' % (r2, o2)
                                                 for r2, o2 in sorted(offs.items())))
                ambiguous[m2['name']] = dict(offs)

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
    # `NATION(u)` is a #define in the file, and `Sprite();` inside `class Sprite {...}`
    # is a constructor DECLARATION. Neither is a call, and demanding a port of them held
    # up 46 files over callees that do not exist.
    file_macros = set(re.findall(r'^\s*#\s*define\s+(\w+)\s*\(', body, re.M))
    file_classes = set(re.findall(r'\bclass\s+(\w+)', blank))
    called = {c for c in re.findall(r'\b([A-Za-z_]\w*)\s*\(', blank_rest)
              if c not in KEYWORDS and c != name
              and c not in file_macros and c not in file_classes
              and c.lstrip('~') not in file_classes}
    runtime = sorted(c for c in called if c in RUNTIME)
    api = sorted(c for c in called if c.upper() in WINAPI and c not in RUNTIME)
    taint = tainted()
    missing = sorted(c for c in called
                     if c not in RUNTIME and c.upper() not in WINAPI
                     and c not in WINMACRO
                     and not ported_paths(c))
    # A callee with no port of its own, but WITH a label in the shipped image, is not a
    # hole: tests/emu_differential.py points the call at the shipped implementation, so
    # the candidate and the shipped function reach the same code and the comparison is
    # still the right experiment. tools/link.py stubs it for the playable build and
    # --report counts it, so nothing is claimed that is not there. The host harness has
    # no such implementation to call, so it does not own these -- hence host_blind.
    from_shipped = [c for c in missing
                    if SHIPPED_ALIAS.get(c, c) in SHIPPED or c in SHIPPED]
    missing = [c for c in missing if c not in from_shipped]
    if missing:
        return None, 'calls %s, which is not ported' % ', '.join(missing[:3])

    local_types = set(typedefs) | set(tags) | {
        m2.group(1) for m2 in re.finditer(r'^\s*class\s+(\w+)', blank, re.M)}
    params = parse_params(d.group('args'), local_types)

    if params is None:
        return None, 'a parameter this cannot generate values for'
    conv = [c for c in CONV if re.search(r'\b%s\b' % c, d.group('head'))]
    # `pascal` reverses the push order, and the emulator pushes right to left. That does
    # NOT break the comparison: both the shipped function and the candidate are pascal,
    # both read the same stack the same way, so the only consequence is that the sweep's
    # first value lands in the last parameter -- a relabelling, and with every scalar
    # swept over the same range not even a narrower sweep. What it does break is a
    # POINTER argument, whose staged address would land in a slot the callee reads as an
    # integer, so a mixed signature is still refused. The host compiles the port without
    # `pascal` and would read the arguments the other way round, so it does not own these.
    pascal_blind = None
    if conv and len(params) > 1:
        if any(t.endswith('*') for t, _ in params):
            return None, '%s with %d arguments and a pointer among them: the emulator ' \
                         'pushes the other way, and the staged address would land in a ' \
                         'slot the callee reads as an integer' % (conv[0], len(params))
        pascal_blind = ('is %s: the emulator pushes the arguments the other way, which '
                        'the shipped function and this port read alike and a host build '
                        'does not' % conv[0])
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
    if nscalar > 5 or len(params) > 7:
        return None, '%d arguments is more than a sweep should enumerate' % len(params)
    # A negative argument is worth sweeping -- it is where signedness shows -- but not
    # into a table. shift_table_down(-4) walks a loop from 14 down to -4 and indexes the
    # global at every step: in 16 bits that wraps inside the segment, and on a host it
    # runs off the front of the array. The index need not be the parameter itself, so the
    # only safe rule is: a function that indexes any global at all is swept from 0.
    lowest = 0 if re.search(r'\bg_\w+\s*\[', blank_rest) else -4
    # And not past the END of what it indexes, either. rebuild_cell_row takes a row and
    # subscripts a global with four rows in it; swept to 24 it read twenty rows past the
    # array, where the game's memory and a host's hold different things -- the 16-bit
    # harness agreed on all twelve sets and the host reported writes the shipped code
    # never made. Reading off the end is not evidence about the port either way.
    highest = 24
    for gn in set(re.findall(r'\b(g_\w+)\s*\[', blank_rest)):
        g = GLOBALS.get(gn)
        if not g or not g.get('dims') or g.get('size') is None or g.get('stars'):
            continue
        stride = RECORDS[g['base']]['size'] if g['base'] in RECORDS \
            else WIDTH.get(g['base'])
        if not stride:
            continue
        for gd in g['dims'][1:]:     # not `d`: that is the definition match, and
            v = dim(gd)                 # shadowing it cost a crash here already
            if v is None:
                stride = None
                break
            stride *= v
        if stride:
            # With a FLOOR. Most of these extents are inferred from the gap to the
            # next global, and a gap smaller than one row says the extent is wrong, not
            # that the array holds one element: thirty-eight cases collapsed to a single
            # argument set on that reading, which is not a sweep at all.
            highest = min(highest, max(3, g['size'] // stride - 1))
    # Four and five scalars are worth sweeping, just over a narrower range: what has to
    # stay bounded is the PRODUCT, not the count. 81 sets and 32, against 64 for two.
    per = {0: [], 1: ['0=%d..%d' % (lowest, highest)], 2: ['0=0..7', '1=0..7'],
           3: ['0=0..3', '1=0..3', '2=0..3'],
           4: ['%d=0..2' % i for i in range(4)],
           5: ['%d=0..1' % i for i in range(5)]}[nscalar]
    # Size the staged region to the POINTEE, not a flat 64 bytes. A record bigger than
    # the block meant the function read past the pattern into whatever followed, which is
    # how the MS*File family came to take an early exit the shipped code does not.
    def pointee_size(t):
        t = t[:-1].strip()
        if t in RECORDS:
            return RECORDS[t]['size']
        lay = local_layout(blank_comments(typedefs.get(t, ''))) if t in typedefs else None
        if lay:
            return max(o + w for o, _n, _t, w, _a, _p in lay)
        return 64
    block = max([64] + [pointee_size(t) for t, _ in params if t.endswith('*')])
    block = (block + 15) & ~15
    base = scratch(block * max(1, npoint) + 512) if npoint else None
    if npoint and base is None:
        return None, 'no room in any data segment to stage %d pointer argument%s' \
            % (npoint, '' if npoint == 1 else 's')
    bounds = index_bounds(rest, params, [n for _, ns in dropped for n in ns])
    sweeps, callargs, stage, k, blk, si = [], [], {}, 0, 0, 0
    for ty, pname in params:
        if ty.endswith('*'):
            pt = ty[:-1].strip()
            off = base[1] + blk * block
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
                        for i in range(block))
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
                r'typedef\s+struct\s*\w*\s*\{.*?\}\s*%s\s*;\s*' % re.escape(t),
                '', body_text, flags=re.S)
            if t in tag_of:
                # `struct Item far *next` inside another struct, and every later use of
                # the tag, now means the record the typedef was mapped onto.
                body_text = re.sub(r'\bstruct\s+%s\b' % re.escape(tag_of[t]),
                                   canon_of[t], body_text)
    # A file-local struct whose name is also a record's. It was not mapped onto that
    # record -- nothing declared with it is a global -- so it stays, and it has to stop
    # colliding with the header's typedef of the same name. Popup_item's own
    # `struct Item` is a list node with next/prev pointers; the header's Item is not the
    # same shape, and leaving both spellings made the file refuse to compile.
    local_only = sorted({t for t in list(typedefs) + list(tags)
                         if t not in canon_of and 'struct ' + t not in canon_of
                         and t.replace('struct ', '') in RECORDS
                         and t not in drop_typedefs})
    for t in local_only:
        bare = t.replace('struct ', '')
        for nm in {bare, tag_of.get(t, bare)}:
            body_text = re.sub(r'\b%s\b' % re.escape(nm), nm + '_local', body_text)
            body_pre = re.sub(r'\b%s\b' % re.escape(nm), nm + '_local', body_pre)

    # An OPAQUE class: `class Sprite { public: char body[0x20]; Sprite(); ~Sprite(); };`
    # is not a class, it is a size. The header already defines the record it stands for,
    # and where it does not, a struct of the same body is the same object to every use in
    # the file. Only this one-line form is taken; a class with real members or a base
    # list is a layout question, and guessing at it would put members at wrong offsets.
    OPAQUE = re.compile(r'^[ \t]*class\s+(\w+)\s*(?::[^{]*)?\{\s*public\s*:\s*'
                        r'char\s+(\w+)\s*\[([^\]]+)\]\s*;\s*'
                        r'(?:\1\s*\(\s*\)\s*;\s*|~\s*\1\s*\(\s*\)\s*;\s*)*'
                        r'\}\s*;[ \t]*\n?', re.M)

    opaque_decls = []

    def _opaque(mo):
        cls, field, size = mo.group(1), mo.group(2), dim(mo.group(3))
        # An INSTANCE of it is not an opaque size, it is a constructor and a destructor.
        # `GRPort port;` on the stack calls GRPort::GRPort() on entry and ~GRPort() on
        # exit, and a plain struct of the same bytes calls neither: seven ReportWin
        # handlers agreed on every write and every returned value while calling no
        # imports at all, because the destructor the port had dropped is what reaches
        # InvalidateRect. Only a class used through a POINTER is a size.
        if re.search(r'\b%s\s+(?!far\b|near\b)\w+\s*[;,\[=]' % re.escape(cls),
                     blank_rest):
            raise Opaque('declares an instance of %s, whose constructor and destructor '
                         'the shipped code calls and a struct of the same bytes does '
                         'not' % cls)
        if cls in RECORDS:
            if size is not None and size != RECORDS[cls]['size']:
                raise Opaque('%s is %d bytes in this file and %d in the model'
                             % (cls, size, RECORDS[cls]['size']))
            return ''                      # col/records.h defines it
        opaque_decls.append('typedef struct { char %s[%s]; } %s;'
                            % (field, mo.group(3), cls))
        return ''                          # hoisted above the stub prototypes, which
                                           # take it as a parameter type

    try:
        body_text = OPAQUE.sub(_opaque, body_text)
        body_pre = OPAQUE.sub(_opaque, body_pre)
    except Opaque as e:
        return None, str(e)
    # And the same question for a class this did NOT rewrite. Whatever shape its
    # declaration has, an instance of it is a constructor and a destructor the port has
    # no way to call.
    for cls in sorted(re.findall(r'\bclass\s+(\w+)', blank)):
        if re.search(r'\b%s\s+(?!far\b|near\b)\w+\s*[;,\[=]' % re.escape(cls),
                     blank_rest):
            return None, 'declares an instance of %s, whose constructor and destructor ' \
                         'the shipped code calls and a struct of the same bytes does ' \
                         'not' % cls

    # Which record each identifier in this file holds. Built AFTER the typedefs are
    # dropped, when every declaration already spells the canonical record name. This is
    # only consulted for a member name that means two different offsets in the records
    # in play; everywhere else a name-keyed rewrite is unambiguous and cheaper.
    var_record = {}
    if ambiguous:
        for rn in RECORDS:
            for m in re.finditer(r'\b%s\b(?:\s+(?:far|near|const))*\s*\**\s*(\w+)'
                                 % re.escape(rn), body_text):
                var_record[m.group(1)] = rn
        for _, gnames in dropped:
            for n in gnames:
                b2 = GLOBALS.get(n, {}).get('base')
                if b2 in RECORDS:
                    var_record[n] = b2

    def recv_record(expr):
        """The record `expr` points at, or None if this cannot say.

        Deliberately narrow: one known identifier and no member chain. A chain would
        need the member's own type, and a wrong answer here puts one record's offset on
        another -- which compiles, and is exactly what the refusal exists to stop.
        """
        if expr is None or '->' in expr or '.' in expr:
            return None
        found = {var_record[i] for i in re.findall(r'\b\w+\b', expr)
                 if i in var_record}
        return found.pop() if len(found) == 1 else None

    def site_wanted(name, text, start):
        """Whether the rule for `name` applies to the receiver at this site."""
        if name not in ambiguous:
            return True
        want = rule_record.get(name)
        got = recv_record(receiver_before(text, start))
        if got is None:
            raise Ambiguous(name)
        return got == want

    for old, new in renames.items():
        pos = 0
        while True:
            m = re.compile(r'(->|\.)(\s*)%s\b' % re.escape(old)).search(body_text, pos)
            if not m:
                break
            try:
                take = site_wanted(old, body_text, m.start())
            except Ambiguous:
                return None, 'the member name %s means %s in this file, and this ' \
                             'receiver is not a plain variable this can type' \
                             % (old, ' and '.join('%s+0x%02x' % kv
                                                  for kv in sorted(ambiguous[old].items())))
            if not take:
                pos = m.end()
                continue
            body_text = body_text[:m.start()] + m.group(1) + m.group(2) + new \
                + body_text[m.end():]
            pos = m.start() + len(m.group(1)) + len(m.group(2)) + len(new)
    for cname_, (off, elem, was_array) in sorted(casts.items()):
        pos = 0
        while True:
            m = re.compile(r'(->|\.)\s*%s\b' % re.escape(cname_)).search(body_text, pos)
            if not m:
                break
            try:
                if not site_wanted(cname_, body_text, m.start()):
                    pos = m.end()
                    continue
            except Ambiguous:
                return None, 'the member name %s means %s in this file, and this ' \
                             'receiver is not a plain variable this can type' \
                             % (cname_,
                                ' and '.join('%s+0x%02x' % kv
                                             for kv in sorted(ambiguous[cname_].items())))
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
            pos = m.start() - len(recv) + len(use)
    def _bracket(text, i):
        """The index just past the `[...]` that starts at `i`, or None."""
        depth = 0
        while i < len(text):
            if text[i] == '[':
                depth += 1
            elif text[i] == ']':
                depth -= 1
                if depth == 0:
                    return i + 1
            i += 1
        return None

    # A global the file declares with one dimension MORE than the header gives it. The
    # header names the row: `char far g_icon_sprites[][0x20]` is `Sprite far *`, and the
    # 0x20 is the record. So one subscript, which in the file is the row's address, is a
    # record VALUE here and has to have its address taken back; two subscripts index
    # bytes within the row, which the record type no longer spells.
    # The mirror of the rule further down: the file declares the global as an ARRAY and
    # the header spells it as one record VALUE -- `char far g_map_window[]` against
    # `(*(MapWin far *)...)`. A bare use in the file is the address of the first byte, so
    # it has to have its address taken back here, and a subscript indexes bytes the
    # record type no longer names.
    for _, names in dropped:
        for n in names:
            g = GLOBALS.get(n, {})
            if g.get('dims') or file_shape.get(n) != 'array' \
                    or g.get('base') not in RECORDS:
                continue
            pos = 0
            while True:
                m = re.compile(r'(&\s*)?\b%s\b' % re.escape(n)).search(body_text, pos)
                if not m:
                    break
                j = m.end()
                while j < len(body_text) and body_text[j] in ' \t':
                    j += 1
                if j < len(body_text) and body_text[j] == '[':
                    e = _bracket(body_text, j)
                    if e is None:
                        break
                    rep = '((unsigned char far *)&%s)%s' % (n, body_text[j:e])
                    end = e
                else:
                    rep = '(&%s)' % n
                    end = m.end()
                body_text = body_text[:m.start()] + rep + body_text[end:]
                pos = m.start() + len(rep)

    for _, names in dropped:
        for n in names:
            g = GLOBALS.get(n, {})
            if not (file_dims.get(n) == 2 and len(g.get('dims') or []) == 1
                    and g.get('base') in RECORDS):
                continue
            pos = 0
            while True:
                m = re.compile(r'\b%s\s*(?=\[)' % re.escape(n)).search(body_text, pos)
                if not m:
                    break
                e1 = _bracket(body_text, body_text.index('[', m.end()))
                if e1 is None:
                    break
                j = e1
                while j < len(body_text) and body_text[j] in ' \t':
                    j += 1
                row = body_text[m.start():e1]
                if j < len(body_text) and body_text[j] == '[':
                    e2 = _bracket(body_text, j)
                    if e2 is None:
                        break
                    inner = body_text[j:e2]
                    rep = '((unsigned char far *)&%s)%s' % (row, inner)
                    end = e2
                else:
                    rep = '(&%s)' % row
                    end = e1
                body_text = body_text[:m.start()] + rep + body_text[end:]
                pos = m.start() + len(rep)

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
    # Spell a shipped-only callee the way the disassembly labels it, so the fixup
    # the emulator resolves carries a name it can find. Both the extern and the
    # call site are rewritten; the declaration is the source file's own.
    for c in from_shipped:
        if c in SHIPPED_ALIAS:
            body_text = re.sub(r'\b%s\b' % re.escape(c), SHIPPED_ALIAS[c],
                               body_text)
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
    # The game has functions of its own called GetPixel and SetPixel -- they take a
    # GRPort, not an HDC -- and windows.h has already declared the GDI ones by the time
    # the unit spells its own. Borland stops at "Type mismatch in redeclaration"; clang
    # never sees it, having no windows.h, so the gate in this tool let those ports
    # through. NOGDI drops the GDI half of windows.h, which these units do not use.
    # tools/port.py --borland names any further collision the same way, in its error.
    if re.search(r'\b(?:GetPixel|SetPixel)\b', ported + ''.join(protos)):
        src += ('/* The game has its own GetPixel and SetPixel, on a GRPort rather than\n'
                ' * an HDC. windows.h declares the GDI pair, which this unit does not\n'
                ' * use, and Borland will not take both spellings of the name. */\n'
                '#define NOGDI\n')
    for h in sorted({RUNTIME[r][1] for r in runtime}):
        src += '#include <%s>\n' % h
    src += '#include "col/globals.h"\n\n'
    # `extern "C"` in front of the definition. Borland builds this unit with -P, where
    # it is what keeps the shipped, unmangled name; the host builds it as C89, where it
    # is a syntax error. Both are right, so the file says so rather than picking one.
    # A unit Borland builds with -P is C++, and a C++ function's name is MANGLED. The
    # shipped symbol is not: colony_end_of_turn is published as `_colony_end_of_turn`
    # there, so the original had C linkage, and without it the 16-bit harness could not
    # find the function at all -- "not in PUBDEFs", counted as a disagreement.
    flags_p = '-P' in (re.search(r'flags:\s*([-\w \t]+)', head).group(1).split()
                       if re.search(r'flags:\s*([-\w \t]+)', head) else [])
    if flags_p and not re.search(r'extern\s*"C"', ported):
        ds = definitions(blank_comments(ported))
        if ds:
            at = ds[0][2]
            ported = ported[:at] + 'COL_EXTERN_C ' + ported[at:]
    if re.search(r'extern\s*"C"', ported) or 'COL_EXTERN_C' in ported:
        ported = re.sub(r'extern\s*"C"', 'COL_EXTERN_C', ported)
        src += ('/* The reconstruction marks this function `extern "C"`: Borland builds\n'
                ' * the unit as C++ and that is what keeps the shipped name unmangled.\n'
                ' * A host build is C, where the same words do not parse. */\n'
                '#ifdef __cplusplus\n'
                '#define COL_EXTERN_C extern "C"\n'
                '#else\n'
                '#define COL_EXTERN_C\n'
                '#endif\n\n')
    if opaque_decls:
        src += ('/* A class the file declares only to give its size a name. The record is\n'
                ' * opaque here for the same reason it is there: nothing in this unit\n'
                ' * reads a member of it. */\n'
                + '\n'.join(sorted(set(opaque_decls))) + '\n\n')
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
    bad = compiles(src, bool(host_blind))
    if bad and not host_blind and any(
            k in bad for k in ('smaller integer type', 'integer to pointer',
                               'pointer to integer', 'incompatible pointer types',
                               'different sign', "from 'char *'", "to 'char *'")):
        # It only fails on pointer WIDTH or SIGNEDNESS. A function that casts between a
        # pointer and a 32-bit integer is not host-representable by construction -- four
        # bytes in the game, eight here -- so the compiler has just told us which oracle
        # owns it. Re-check with those diagnostics relaxed and hand it to the 16-bit one.
        again = compiles(src, True)
        if not again:
            host_blind = ('casts between a pointer and an integer, which a host cannot '
                          'represent: a far pointer is 4 bytes in the game and 8 here')
            bad = None
        else:
            bad = again
    if bad:
        return None, 'the port does not compile: %s' % bad

    args = ', '.join(callargs)
    case = {'what': 'ported by tools/port.py; compared against the shipped bytes',
            'symbol': name, 'shipped': shipped.group(1),
            'source': 'src/%s/%s.%s' % (os.path.basename(os.path.dirname(path)), name,
                                        'cpp' if is_cpp else 'c'),
            'prototype': sig + ';',
            'state': 'shipped', 'ret_bits': INTEGRAL[ret],
            # What the 16-bit harness compares, when it differs: a returned far pointer is
            # a real 32-bit value there and meaningless on a host.
            'ret_bits_emu': 32 if ret_is_ptr else INTEGRAL[ret],
            'sweep': sweeps, 'stage': stage,
            'call': ('(long)%s(%s)' if INTEGRAL[ret] else '(void)%s(%s)'
                     if ret_is_ptr else '%s(%s)') % (name, args),
            'globals_read': sorted({n for _, names in dropped for n in names}),
            # The flags this function's own reconstruction was matched with.
            'flags': (re.search(r'flags:\s*([-\w \t]+)', head).group(1).strip()
                      if re.search(r'flags:\s*([-\w \t]+)', head) else
                      '-ml -WS -Od -Ot -3'),
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
    if pascal_blind and not host_blind:
        host_blind = pascal_blind
    if from_shipped and not host_blind:
        host_blind = ('calls %s, which this repo has not ported: the 16-bit harness '
                      'points the call at the shipped implementation, and a host has '
                      'none' % ', '.join(sorted(from_shipped)[:3]))
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


def compiles(text, host_blind=False):
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
        # A host-blind port is never RUN on the host, only compiled so the archive
        # builds. Its pointer/integer casts are exact in 16 bits and impossible on a
        # 64-bit host, so those diagnostics say nothing about the port -- everything else
        # still has to compile clean.
        relax = ['-Wno-error=int-conversion',
                 '-Wno-error=int-to-pointer-cast',
                 '-Wno-error=pointer-to-int-cast',
                 '-Wno-error=incompatible-pointer-types',
                 '-Wno-error=pointer-integer-compare',
                 '-Wno-error=shorten-64-to-32',
                 '-Wno-error=pointer-sign',
                 '-Wno-error=incompatible-pointer-types-discards-qualifiers',
                 '-Wno-error=int-to-void-pointer-cast',
                 '-Wno-error=void-pointer-to-int-cast'] if host_blind else []
        r = subprocess.run(
            [os.environ.get('CC', 'cc'), '-std=c89', '-DCOL_HOST=1', '-fsigned-char',
             '-fwrapv', '-fno-strict-aliasing', '-Wno-unused-value', '-Werror'] + relax +
            ['-fsyntax-only', '-I' + os.path.join(REPO, 'include'), tmp],
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
    dst = os.path.join(REPO, 'src', sub,
                       name + ('.cpp' if case.get('source', '').endswith('.cpp') else '.c'))
    if os.path.exists(dst) and not force:
        return False, 'already ported'
    # --force re-makes what THIS TOOL made. A port someone wrote or corrected by hand is
    # not a stale generated file, and a sweep of --force over the corpus overwrote five
    # of them -- including job_input_goods, whose whole point is that it departs from a
    # reconstruction with two jump-table arms the wrong way round. The generated port put
    # the defect back. --prune already draws this line; --force has to draw the same one.
    if os.path.exists(dst) and 'Ported by tools/port.py' not in open(dst).read():
        return False, 'a hand-written port: not this tool\'s to overwrite'
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
    ap.add_argument('--roots', action='store_true',
                    help='the refusals every other refusal rests on')
    ap.add_argument('--batch', type=int)
    ap.add_argument('--force', action='store_true')
    ap.add_argument('--borland', action='store_true',
                    help='drop ports the real toolchain rejects')
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

    if a.roots:
        # A refusal that reads "calls X, which is not ported" is not a reason, it is a
        # forwarding address. Follow every chain to the refusals that are NOT cascades,
        # and rank those by how many files each one holds up. That is the worklist: one
        # root here is worth twenty of the files that name it.
        import collections
        reason_of, calls = {}, {}
        for path in corpus:
            nm = os.path.splitext(os.path.basename(path))[0]
            src, reason = port(path)
            if src is not None:
                continue
            m = re.match(r'calls (.+), which is not ported$', reason)
            if m:
                calls[nm] = [c.strip() for c in m.group(1).split(',')]
            reason_of[nm] = reason

        def roots_of(nm, seen):
            if nm in seen:
                return set()
            seen = seen | {nm}
            if nm not in calls:
                return {nm} if nm in reason_of else set()
            out = set()
            for c in calls[nm]:
                out |= roots_of(c, seen)
            return out or {nm}

        blocked = collections.Counter()
        for nm in reason_of:
            for r in roots_of(nm, set()):
                blocked[r] += 1
        print('%d refusals, resting on %d roots' % (len(reason_of), len(blocked)))
        for r, n in blocked.most_common(25):
            print('  %4d  %-30s %s' % (n, r, reason_of.get(r, 'no reconstruction')))
        return 0

    if a.borland:
        # The compile gate in port() is clang, which is a PROXY for the toolchain that
        # matters. Borland rejects things clang accepts -- a unit whose function shares a
        # name with a Windows API, so windows.h has already declared it with another
        # signature; an incomplete type used by value -- and those ports reached src/ and
        # the suite, where they read as disagreements rather than as what they are.
        # Batched, because a DOSBox session costs seconds and a compile costs a fraction.
        sys.path.insert(0, os.path.join(REPO, 'tools'))
        import bcc
        ports = sorted(glob.glob(os.path.join(REPO, 'src', '*', '*.c'))
                       + glob.glob(os.path.join(REPO, 'src', '*', '*.cpp')))
        ports = [x for x in ports
                 if 'Ported by tools/port.py' in open(x).read()]
        print('compiling %d ports with Borland C++ 4.52' % len(ports))
        gone = 0
        by_flags = {}
        for x in ports:
            nm = os.path.splitext(os.path.basename(x))[0]
            cpath = os.path.join(REPO, 'tests', 'cases', nm + '.json')
            fl = '-ml -WS -Od -Ot -3'
            if os.path.exists(cpath):
                fl = json.load(open(cpath)).get('flags', fl)
            by_flags.setdefault(fl, []).append(x)
        for fl, group in sorted(by_flags.items()):
            for x, res in bcc.compile_all(group, fl).items():
                obj, log = res[0], (res[1] if len(res) > 1 else '')
                if obj is not None:
                    continue
                why = next((l.strip() for l in (log or '').splitlines()
                            if l.strip().startswith('Error')), 'Borland rejected it')
                nm = os.path.splitext(os.path.basename(x))[0]
                for f in (x, os.path.join(REPO, 'tests', 'cases', nm + '.json')):
                    if os.path.exists(f):
                        os.remove(f)
                print('%-28s dropped: %s' % (nm, why[:90]))
                gone += 1
        print('%d dropped' % gone)
        return 0

    if a.prune:
        gone = 0
        for p in corpus:
            name = os.path.splitext(os.path.basename(p))[0]
            # .cpp TOO. Looking only for .c meant every C++ port was invisible to
            # --prune: twenty-seven of them stopped being ports this tool would make and
            # all twenty-seven stayed in src/ and in the suite.
            here = ported_paths(name)
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
            if ported_paths(name):
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
