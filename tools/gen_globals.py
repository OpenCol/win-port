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
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import gen_headers as gh      # noqa: E402

OUT = os.path.join(ROOT, "src", "gen", "globals.c")
HEADER = os.path.join(ROOT, "include", "gen", "colonization.h")

# one element's bytes in the shipped game (Borland C++ 4.52, large model)
GAME_WIDTH = {"char": 1, "signed char": 1, "unsigned char": 1, "int8_t": 1, "uint8_t": 1,
              "short": 2, "unsigned short": 2, "int16_t": 2, "uint16_t": 2, "i16": 2, "u16": 2,
              "int": 2, "unsigned": 2, "unsigned int": 2, "long": 4, "unsigned long": 4,
              "int32_t": 4, "uint32_t": 4, "i32": 4, "u32": 4, "float": 4, "double": 8}
VAR = re.compile(r"^extern\s+(?!\"C\")((?:const\s+)?[A-Za-z_][\w\s]*?)\s*(\*+\s*)?\b(\w+)\s*((?:\[[^\]]*\]\s*)*);")
FNPTR = re.compile(r"^extern\s+(?!\"C\")[^;(){}]*\(\s*\*\s*(\w+)\s*\)\s*\([^;{}]*\)\s*;")


def declarations():
    """{name: declaration} of the variables the header declares at file scope (C side)."""
    text = gh.strip_comments(open(HEADER).read())
    # the C++-only blocks: globals.c compiles as C
    text = re.sub(r"#ifdef __cplusplus\n.*?#(?:else|endif)", "", text, flags=re.S)
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


def generate():
    layout = json.load(open(os.path.join(ROOT, "import", "layout.json")))
    ext = extents(layout)
    records = layout["records"]
    decls = declarations()
    defined = set(gh.global_definitions())
    body, problems = [], []
    for name in sorted(decls):
        if name in defined:
            continue
        text, why = definition(decls[name], name, ext, records)
        if text:
            body.append(text)
        else:
            problems.append("%s: %s" % (name, why))
    head = ["/* GENERATED by tools/gen_globals.py from include/gen/colonization.h and import/layout.json.",
            " * Do not edit: run the generator. Storage for each global the port declares and no",
            " * source defines; an open array gets the game's element count (see the generator).",
            " */",
            '#include "colonization.h"', ""]
    tail = [""] + (["/* not defined here (%d):" % len(problems)] + [" *   " + p for p in problems] + [" */"]
                   if problems else [])
    return "\n".join(head + body + tail) + "\n", len(body), problems


def main(argv):
    text, n, problems = generate()
    if "--check" in argv:
        have = open(OUT).read() if os.path.exists(OUT) else ""
        if have != text:
            print("src/gen/globals.c is not what tools/gen_globals.py writes")
            return 1
        print("src/gen/globals.c: current")
        return 0
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    open(OUT, "w").write(text)
    print("src/gen/globals.c: %d definitions; %d not defined" % (n, len(problems)))
    for p in problems[:20]:
        print("  " + p)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
