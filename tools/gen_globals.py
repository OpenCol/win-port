#!/usr/bin/env python3
"""Storage for every global the port declares and no source defines (roadmap W2).

    python3 tools/gen_globals.py            # write src/gen/globals.c
    python3 tools/gen_globals.py --check    # fail if src/gen/globals.c is not what this writes

The reconstructions declare the game's globals and almost never define them: in the shipped
executable they are DGROUP and data-segment bytes no one function owns. The port gives each
one storage here, typed exactly as include/gen/colonization.h (or, for a global only the
files declare, the one way they spell it) declares it.

An array declared with an open first dimension (`Unit g_units[]`) needs a count, and no
file knows it. It comes from the layout (import/layout.json, from col-win-re's model and
the NE segment table): a global runs to the next global in its segment, or to the
segment's end, and the count is that extent over one element's size in the GAME's widths
(Borland's int is 2 bytes, a far pointer 4, a record the size the model gives it). So the
port has as many elements as the game had; their port widths may differ, which W3's state
mapping deals with. A global with no address or no element size is listed and left out.

This is col-win-re's tools/gen_port_globals.py, reworked for the port's header.
"""
import json
import os
import re
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import gen_headers as gh      # noqa: E402

OUT = os.path.join(ROOT, "src", "gen", "globals.c")
OUT_CXX = os.path.join(ROOT, "src", "gen", "globals_cxx.c")
HEADER = os.path.join(ROOT, "include", "gen", "colonization.h")

# one element's bytes in the shipped game (Borland C++ 4.52, large model)
GAME_WIDTH = {"char": 1, "signed char": 1, "unsigned char": 1, "int8_t": 1, "uint8_t": 1,
              "short": 2, "unsigned short": 2, "int16_t": 2, "uint16_t": 2, "i16": 2, "u16": 2,
              "int": 2, "unsigned": 2, "unsigned int": 2, "long": 4, "unsigned long": 4,
              "int32_t": 4, "uint32_t": 4, "i32": 4, "u32": 4, "float": 4, "double": 8}
VAR = re.compile(r"^extern\s+(?!\"C\")((?:const\s+)?[A-Za-z_][\w\s]*?)\s*(\*+\s*)?\b(\w+)\s*((?:\[[^\]]*\]\s*)*);")
FNPTR = re.compile(r"^extern\s+(?!\"C\")[^;(){}]*\(\s*\*\s*(\w+)\s*\)\s*\([^;{}]*\)\s*;")


def declarations(cpp=False):
    """{name: declaration} of the variables the header declares at file scope, to C files or
    (cpp) only to C++ files."""
    text = gh.strip_comments(open(HEADER).read())
    if cpp == "all":       # everything a C++ file sees: the shared text and the C++-only blocks
        text = re.sub(r"#ifndef __cplusplus\n.*?#endif", "", text, flags=re.S)
        text = re.sub(r"#ifdef __cplusplus\n(.*?)#else\n.*?#endif", r"\1", text, flags=re.S)
    else:
        blocks = re.findall(r"#ifdef __cplusplus\n(.*?)#(?:else|endif)", text, flags=re.S)
        text = "\n".join(blocks) if cpp else re.sub(r"#ifdef __cplusplus\n.*?#(?:else|endif)", "", text, flags=re.S)
    out = {}
    for line in text.splitlines():
        line = line.strip()
        m = VAR.match(line)
        if m and "(" not in line:
            out[m.group(3)] = line
            continue
        m = FNPTR.match(line)
        if m:
            out[m.group(1)] = line
    return out


def game_width(base, stars, records):
    if stars:
        return 4
    base = re.sub(r"\s+", " ", base.replace("const ", "")).strip()
    if base in GAME_WIDTH:
        return GAME_WIDTH[base]
    return records.get(base)


def extents(layout):
    per = {}
    for name, (seg, off) in layout["globals"].items():
        per.setdefault(seg, set()).add(off)
    nxt = {}
    for seg, offs in per.items():
        offs = sorted(offs)
        end = layout["segments"].get(str(seg))
        for i, o in enumerate(offs):
            nxt[(seg, o)] = offs[i + 1] if i + 1 < len(offs) else end
    return {name: nxt[(seg, off)] - off for name, (seg, off) in layout["globals"].items()
            if nxt.get((seg, off)) and nxt[(seg, off)] > off}


def definition(decl, name, ext, records):
    """(definition, None) or (None, why)."""
    d = decl[len("extern"):].strip()
    m = VAR.match(decl)
    if not m:
        return d, None                        # a function pointer: defined as declared
    base, stars, dims = m.group(1), (m.group(2) or "").replace(" ", ""), m.group(4).replace(" ", "")
    inner = re.findall(r"\[([^\]]*)\]", dims)
    if not inner or inner[0].strip():
        return d, None                        # a scalar, or an array with its size
    w = game_width(base, stars, records)
    if not w:
        return None, "no game width for `%s`" % base.strip()
    for x in inner[1:]:
        w *= int(x, 0)
    if name not in ext:
        return None, "no address in the model"
    n = max(1, ext[name] // w)
    sized = "[%d]%s" % (n, "".join("[%s]" % x for x in inner[1:]))
    return "%s %s%s%s;" % (base.strip(), stars, name, sized), None


# Objects that hold other modelled globals: in the game g_map_window, g_stat_win and the other
# child windows are members of the main window, at fixed offsets. The port lays MainWin out as
# the game does (asserted in create_game_globals.c), so each contained global is an alias,
# `name = container + offset`, given to the linker (src/gen/aliases.txt), never storage of its own.
CONTAINERS = {"g_main_win": 0x3b0c}     # MainWin, its type descriptor's size
OUT_ALIASES = os.path.join(ROOT, "src", "gen", "aliases.txt")


def aliases():
    """{name: (container, offset)} for every addressed global inside a container."""
    layout = json.load(open(os.path.join(ROOT, "import", "layout.json")))
    addr = dict(json.load(open(os.path.join(ROOT, "import", "addresses.json"))))
    addr.update(layout["globals"])
    out = {}
    for c, size in CONTAINERS.items():
        cseg, coff = addr[c]
        for n, (seg, off) in addr.items():
            if n != c and seg == cseg and coff < off < coff + size:
                out[n] = (c, off - coff)
    return out


def file_only():
    """{name: declaration} of globals only the files declare, all one way (spacing aside)."""
    own_vars, _f = gh.file_declarations()
    out = {}
    for name, spells in own_vars.items():
        norm = {re.sub(r"\s*\*\s*", " *", d).replace("* ", "*") for d in spells}
        # `void *` beside one typed pointer is the same storage, untyped: the typed one stands
        typed = {d for d in norm if not re.match(r"extern (?:const )?void \*", d)}
        if len(norm) > 1 and len(typed) == 1 and all("*" in d for d in norm):
            norm = typed
        if name.startswith("g_") and len(norm) == 1:
            out[name] = next(iter(norm))
    return out


def body_for(decls, skip, ext, records):
    body, problems = [], []
    for name in sorted(decls):
        if name in skip:
            continue
        text, why = definition(decls[name], name, ext, records)
        if text:
            body.append(text)
        else:
            problems.append("%s: %s" % (name, why))
    return body, problems


HEAD = ["/* GENERATED by tools/gen_globals.py from include/gen/colonization.h and import/layout.json.",
        " * Do not edit: run the generator. Storage for each global the port declares and no",
        " * source defines; an open array gets the game's element count (see the generator).%s",
        " */"]


def render(head, body, problems, extra=""):
    tail = [""] + (["/* not defined here (%d):" % len(problems)] + [" *   " + p for p in problems] + [" */"]
                   if problems else [])
    lines = [HEAD[0], HEAD[1], HEAD[2] % extra, HEAD[3]] + head + [""]
    return "\n".join(lines + body + tail) + "\n"


def generate():
    """{path: text} of the generated files, and counts."""
    layout = json.load(open(os.path.join(ROOT, "import", "layout.json")))
    ext = extents(layout)
    records = layout["records"]
    defined = set(gh.global_definitions())
    c_decls = declarations()
    cxx_all = declarations(cpp=True)
    # a global whose type is a C++ class is the class's size, not its C view's (a byte view
    # can be 1 byte -- `int8_t g_colony_window` -- or a 16-bit picture of it): the storage
    # is defined where C++ sees the class
    classes = set(gh.CORPUS_CLASSES) | set(re.findall(r"\bclass\s+(\w+)\s*(?::[^{;]*)?\{",
                                                     gh.strip_comments(open(os.path.join(ROOT, "include", "classes.h")).read())))
    moved = {}
    for n, d in list(cxx_all.items()) + list(c_decls.items()):
        m = VAR.match(d)
        if m and m.group(1).strip() in classes and not (m.group(2) or "").strip():
            moved.setdefault(n, cxx_all.get(n, d))
    for n in moved:
        c_decls.pop(n, None)
    cxx_decls = {n: d for n, d in cxx_all.items() if n not in c_decls}
    cxx_decls.update(moved)
    known_c, _known_cpp = gh.known_types(open(os.path.join(ROOT, "include", "colonization.h")).read())
    # globals no header declares: C when C knows the type, else C++ (a pointer to a class)
    for name, d in file_only().items():
        if name in c_decls or name in cxx_decls or name in defined:
            continue
        types = gh.type_names(d, name)
        (c_decls if types <= known_c else cxx_decls)[name] = d
    al = aliases()
    c_body, c_prob = body_for(c_decls, defined | set(al), ext, records)
    x_body, x_prob = body_for(cxx_decls, defined | set(al), ext, records)
    rows, _names, frows = init_table()
    drop = set()
    for _round in range(8):
        text = render_init(rows, frows, drop)
        with tempfile.TemporaryDirectory() as tmp:
            src = os.path.join(tmp, "globals_init.c")
            open(src, "w").write(text)
            r = subprocess.run(["i686-w64-mingw32-g++", "-x", "c++", "-std=gnu++03", "-fsyntax-only", "-w",
                                "-I", os.path.join(ROOT, "include", "gen"), "-I", os.path.join(ROOT, "include"), src],
                               capture_output=True, text=True)
        if r.returncode == 0:
            break
        lines = text.splitlines()
        bad = set()
        for m in re.finditer(r"globals_init\.c:(\d+):\d+: error", r.stderr):
            row = lines[int(m.group(1)) - 1]
            n = re.search(r"/\* (\w+) \*/\s*$", row) or re.search(r"\(void \*\)&?(\w+)", row)
            if n:
                bad.add(n.group(1))
        if not bad:
            sys.exit("gen_globals: globals_init.c does not compile:\n" + r.stderr[-1500:])
        drop |= bad
    alias_text = "".join("%s %s 0x%x\n" % (n, c, o) for n, (c, o) in sorted(al.items()))
    files = {
        OUT_ALIASES: "# GENERATED by tools/gen_globals.py: name container offset -- each global inside a\n"
                     "# container object is that object's member, linked as name = container + offset\n" + alias_text,
        OUT_INIT: render_init(rows, frows, drop),
        OUT: render(['#include "colonization.h"'], c_body, c_prob),
        OUT_CXX: render(['#include "colonization.h"', '#include "classes.h"', "", 'extern "C" {'],
                        x_body + ["", '}  /* extern "C" */'], x_prob,
                        "\n * flags: -P\n * The globals whose types only C++ files see (classes): compiled as C++, C linkage."),
    }
    return files, len(c_body) + len(x_body), c_prob + x_prob


# ------------------------------------------------------------------ initial values

OUT_INIT = os.path.join(ROOT, "src", "gen", "globals_init.c")
INIT_KIND = {"bytes": 0, "int": 1, "uint": 2, "pointer": 3}


def function_addresses():
    """{name: (segment selector, offset)} for every function src/ defines at a known address
    (its file's `candidate for SEG:OFF` header), C linkage only."""
    out = {}
    defined = set(gh.definition_heads())      # a C function src/ defines (not a member, not removed)
    for p in gh.src_files():
        if os.path.splitext(os.path.basename(p))[0] not in defined:
            continue
        t = open(p, errors="replace").read(4000)
        m = re.search(r"candidate for (10[0-9a-f]{2}):([0-9a-f]{4})", t)
        if not m:
            continue
        name = os.path.splitext(os.path.basename(p))[0]
        out[name] = (int(m.group(1), 16), int(m.group(2), 16))
    return out


def init_table():
    """The C++ text of src/gen/globals_init.c: each addressed global that has no initializer in
    src/ -- where the game's image gives it its first value -- and each function's address."""
    layout = json.load(open(os.path.join(ROOT, "import", "layout.json")))
    decls = declarations(cpp="all")
    for n, d in file_only().items():
        decls.setdefault(n, d)
    defs = gh.global_definitions()
    initialised = set()
    for p in gh.src_files():
        for m in re.finditer(r"^[A-Za-z_][^;(){}]*?\b(g_\w+)\s*(?:\[[^\]]*\]\s*)*=", gh.file_scope(open(p, errors="replace").read()), re.M):
            initialised.add(m.group(1))
    ext = extents(layout)
    contained = set(aliases())
    rows, names = [], []
    for name, (seg, off) in sorted(layout["globals"].items()):
        d = decls.get(name)
        if not d or name in initialised or name in contained:
            continue
        m = VAR.match(d)
        if not m:
            continue
        base = re.sub(r"\s+", " ", m.group(1).replace("const ", "")).strip()
        stars = (m.group(2) or "").replace(" ", "")
        dims = re.findall(r"\[([^\]]*)\]", m.group(4))
        inner = 1
        for x in dims[1:]:
            inner *= int(x, 0)
        w = game_width(base, stars, layout["records"])
        if not w:
            continue
        w *= inner
        if dims:
            count = (ext.get(name, w) // w) if not dims[0].strip() else int(dims[0], 0)
            elem = "sizeof(%s[0])" % name
        else:
            count, elem = 1, "sizeof(%s)" % name
        count = max(1, count)
        if stars and inner == 1:
            kind = "pointer"
        elif base in ("int", "unsigned", "unsigned int", "signed", "long", "unsigned long", "short", "unsigned short"):
            kind = "uint" if base.startswith("unsigned") else "int"
        else:
            kind = "bytes"
        rows.append('  { (void *)&%s, %d, 0x%04x, %d, %d, %s, %d },   /* %s */' % (name, seg, off, count, w, elem,
                                                                        INIT_KIND[kind], name))
        names.append(name)
    fns = function_addresses()
    frows = ['  { 0x%04x%04xu, (void *)%s },' % (sel, off, n) for n, (sel, off) in sorted(fns.items(), key=lambda kv: kv[1])]
    return rows, names, frows


def render_init(rows, frows, drop=()):
    head = ["/* GENERATED by tools/gen_globals.py: where each global's first value is in the game's image,",
            " * and where each function is. Addresses and sizes only -- the values themselves are read",
            " * from the player's COLONIZE.EXE at startup (src/host/w16_globals_init.c).",
            " * flags: -P",
            " */",
            '#include "colonization.h"', '#include "classes.h"', "", 'extern "C" {',
            "struct PortInitGlobal { void *addr; unsigned short seg, off, count, w16, wn, kind; };",
            "struct PortInitFunction { unsigned long addr16; void *fn; };",
            "extern const struct PortInitGlobal port_init_globals[];", "extern const unsigned port_init_global_count;",
            "extern const struct PortInitFunction port_init_functions[];", "extern const unsigned port_init_function_count;",
            "const struct PortInitGlobal port_init_globals[] = {"]
    rows = [r for r in rows if r.rsplit("/* ", 1)[1].rstrip(" */") not in drop]
    frows = [r for r in frows if r.split("(void *)")[1].rstrip(" },") not in drop]
    mid = ["};", "const unsigned port_init_global_count = %d;" % len(rows), "",
           "const struct PortInitFunction port_init_functions[] = {"]
    tail = ["};", "const unsigned port_init_function_count = %d;" % len(frows), "}", ""]
    return "\n".join(head + rows + mid + frows + tail)


def main(argv):
    files, n, problems = generate()
    if "--check" in argv:
        for p, text in files.items():
            have = open(p).read() if os.path.exists(p) else ""
            if have != text:
                print("%s is not what tools/gen_globals.py writes" % os.path.relpath(p, ROOT))
                return 1
        print("src/gen: current")
        return 0
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    for p, text in files.items():
        open(p, "w").write(text)
    print("src/gen: %d definitions; %d not defined" % (n, len(problems)))
    for p in problems[:20]:
        print("  " + p)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
