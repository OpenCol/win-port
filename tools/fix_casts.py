#!/usr/bin/env python3
"""Write out the pointer conversions Borland C++ made implicitly (W1).

    python3 tools/fix_casts.py            # edit src/, keeping each edit that helps
    python3 tools/fix_casts.py --dry      # list the casts it would write

Borland C++ 4.52 converted between pointer types without a word: `char *` to
`unsigned char *`, a function to `void *`, `const void *` to `void *`, one record pointer to
another. A modern C++ compiler refuses those ("invalid conversion", "cannot convert"), and GCC
14+ refuses the C ones too ("incompatible pointer type"). On 32-bit x86 each is the same bits,
so the port spells the conversion out: the expression GCC points at becomes `(T)(expr)`,
with T the type it wanted.

Only pointer-to-pointer conversions are written (a type with `*` or a function pointer on
both sides); an integer made from a pointer, or the reverse, is left for a person. Positions
come from GCC's JSON diagnostics in bytes. The file gets one `PORT:` note listing the lines.
An edit is kept when the file compiles after it, or has no error it did not have before.
"""
import json
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import compile_all as ca      # noqa: E402
import dedupe_types as dt     # noqa: E402

CONV = re.compile(r"(?:invalid conversion from|cannot convert) '([^']+)'(?: \{aka '[^']+'\})? to '([^']+)'")
INCOMPAT = re.compile(r"(?:passing argument \d+ of '[^']+'|assignment to '[^']+' from|initialization of '[^']+' from) "
                      r"(?:from )?incompatible pointer type")
EXPECTED = re.compile(r"expected '([^']+)'(?: \{aka '[^']+'\})? but argument is of type '([^']+)'")


def is_pointer(t):
    t = re.sub(r"\{aka [^}]*\}", "", t)
    return "*" in t and not re.search(r"\b(int|long|short|char|unsigned)\s*$", t.replace("*", "").strip()) \
        or "(*)" in t


def diagnostics(path):
    cc, cxx, extra = ca.TOOLCHAINS["mingw-i686"]
    cpp = ca.is_cpp(path)
    cmd = ([cxx, "-x", "c++", "-std=gnu++03"] if cpp else [cc, "-x", "c", "-std=gnu99"]) + extra + [
        "-fsyntax-only", "-Wall", "-fdiagnostics-format=json", "-fdiagnostics-column-unit=byte",
        "-I", os.path.join(ROOT, "include", "gen"), "-I", os.path.join(ROOT, "include"), path]
    r = subprocess.run(cmd, capture_output=True, text=True, errors="replace")
    try:
        return json.loads(r.stderr or "[]")
    except ValueError:
        return []


def casts(path):
    """[(line, start byte, end byte, type)] for the pointer conversions GCC rejects in path."""
    out = []
    for d in diagnostics(path):
        if d.get("kind") != "error":
            continue
        msg = d.get("message", "")
        target = None
        m = CONV.search(msg)
        if m and is_pointer(m.group(1)) and is_pointer(m.group(2)):
            target = m.group(2)
        elif INCOMPAT.search(msg):
            for ch in d.get("children", []):
                e = EXPECTED.search(ch.get("message", ""))
                if e and is_pointer(e.group(1)) and is_pointer(e.group(2)):
                    target = e.group(1)
            if target is None:
                m2 = re.search(r"from incompatible pointer type '([^']+)'", msg)
                t2 = re.search(r"assignment to '([^']+)'|initialization of '([^']+)'", msg)
                if t2:
                    target = t2.group(1) or t2.group(2)
        if not target:
            continue
        for loc in d.get("locations", []):
            s, f = loc.get("start") or loc.get("caret"), loc.get("finish") or loc.get("caret")
            if not s or os.path.abspath(s["file"]) != os.path.abspath(path) or s["line"] != f["line"]:
                continue
            out.append((s["line"], s["byte-column"] if "byte-column" in s else s["column"],
                        f["byte-column"] if "byte-column" in f else f["column"], target))
            break
    return out


def apply(text, found):
    lines = text.split("\n")
    done = []
    # right to left within a line, so earlier columns stay valid
    for line, a, b, ty in sorted(set(found), key=lambda x: (x[0], -x[1])):
        s = lines[line - 1]
        expr = s[a - 1:b]
        if not expr.strip() or expr.startswith("(%s)" % ty):
            continue
        lines[line - 1] = s[:a - 1] + "(%s)(%s)" % (ty, expr) + s[b:]
        done.append(line)
    return "\n".join(lines), sorted(set(done))


def main(argv):
    kept, restored = [], []
    for p in ca.sources():
        r0 = ca.compile_one("mingw-i686", p)
        if r0["ok"]:
            continue
        found = casts(p)
        if not found:
            continue
        text = open(p).read()
        new, lines = apply(text, found)
        if not lines:
            continue
        if "--dry" in argv:
            print("%-48s %d cast(s) at %s" % (os.path.relpath(p, ROOT), len(lines), lines))
            continue
        note = ("/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), "
                "lines %s of the imported file. */\n" % ", ".join(map(str, lines)))
        open(p, "w").write(note + new)
        r1 = ca.compile_one("mingw-i686", p)
        if r1["ok"] or dt.error_set(r1) <= dt.error_set(r0):
            kept.append((p, len(lines), r1["ok"]))
        else:
            open(p, "w").write(text)
            restored.append((p, r1["first_raw"]))
    if "--dry" in argv:
        return 0
    print("cast edits kept in %d files (%d now compile); restored %d" % (
        len(kept), sum(1 for k in kept if k[2]), len(restored)))
    for p, why in restored[:20]:
        print("  restored %-44s %s" % (os.path.relpath(p, ROOT), (why or "")[:80]))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
