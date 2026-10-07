#!/usr/bin/env python3
"""C++ files call the game's C functions with C linkage (roadmap W2).

    python3 tools/c_linkage.py --dry    # list the declarations it would change
    python3 tools/c_linkage.py          # change them, keeping each edit that compiles

A C++ reconstruction that declares a callee itself -- `extern void sound_play(int n);` --
gives the call C++ linkage, so the object asks for `sound_play(int)` mangled, while the
function is defined in a C file as plain `_sound_play`. Borland's C++ did the same and
its linker matched them by the corpus's naming discipline; a modern linker does not.

From the link (tools/undefined.py), each undefined mangled name whose plain C symbol some
object defines is a declaration to fix. In the files the linker names, the file-scope
declarations of that function become `extern "C"`; where the header already declares the
function for C++ (a second declaration would conflict), the file's own one is removed and
the header's is used, with the pointer casts the callers need (tools/fix_casts.py). Each
file is kept when it compiles, or gains no error it did not have.
"""
import collections
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import compile_all as ca      # noqa: E402
import dedupe_types as dt     # noqa: E402
import fix_casts as fc        # noqa: E402
import twins                  # noqa: E402
import undefined as und       # noqa: E402


def defined_c_symbols():
    """The plain (C) function names some object defines."""
    out = set()
    for o in [und.obj_path(s) for s in und.sources()]:
        r = subprocess.run([und.NM, "-g", "--defined-only", o], capture_output=True, text=True)
        for line in r.stdout.splitlines():
            parts = line.split()
            if len(parts) == 3 and parts[1] == "T" and parts[2].startswith("_") and not parts[2].startswith("__Z"):
                out.add(parts[2][1:])
    return out


def wanted():
    """{source path: {function name}} -- mangled references to functions defined in C."""
    _srcs, _failed, undefined, _twice, _ok = und.measure()
    names = und.demangle(undefined)
    c_defs = defined_c_symbols()
    out = collections.defaultdict(set)
    for sym, files in undefined.items():
        d = names.get(sym, sym)
        if not sym.startswith("__Z") or "::" in d.split("(")[0]:
            continue
        base = d.split("(")[0].strip()
        if base in c_defs:
            for f in files:
                out[os.path.join(ROOT, "src", f)].add(base)
    return out


def cpp_definitions(bases):
    """{source path: {name}} -- the C++ files that define one of `bases` with C++ linkage."""
    out = collections.defaultdict(set)
    for src in und.sources():
        o = und.obj_path(src)
        r = subprocess.run([und.NM, "-g", "--defined-only", o], capture_output=True, text=True)
        syms = [l.split()[2] for l in r.stdout.splitlines() if len(l.split()) == 3 and l.split()[1] == "T"]
        for sym, d in und.demangle([x for x in syms if x.startswith("__Z")]).items():
            base = d.split("(")[0].strip()
            if base in bases:
                out[src].add(base)
    return out


def define_c(text, funcs):
    """text with the file-scope definitions of `funcs` given C linkage."""
    _t, defs = twins.definitions_text(text)
    cuts = []
    for name, _n, s, _e, _body in defs:
        if name in funcs and not re.match(r'\s*extern\s+"C"', text[s:]):
            cuts.append(s)
    out = text
    for s in sorted(cuts, reverse=True):
        out = out[:s] + 'extern "C"   /* PORT: C linkage: C files call it too (W2) */\n' + out[s:]
    return out, len(cuts)


def header_cpp_names():
    """Functions include/gen/colonization.h declares to C++ callers."""
    t = open(os.path.join(ROOT, "include", "gen", "colonization.h")).read()
    t = re.sub(r"#ifndef __cplusplus\n.*?#endif", "", t, flags=re.S)
    return set(re.findall(r"^extern\s+[^;(){}]*?\b(\w+)\s*\([^;{}]*\)\s*;", t, re.M))


def rewrite(text, funcs, declared):
    b = twins.blank(text)
    depth, depth_at = 0, []
    for ch in b:
        depth_at.append(depth)
        depth += (ch == "{") - (ch == "}")
    cuts = []
    for m in re.finditer(r"^[ \t]*((?:extern\s+)?(?!extern\s+\"C\")[A-Za-z_][^;{}()#=]*?\b(\w+)\s*\([^;{}]*\)\s*;)",
                         b, re.M):
        name = m.group(2)
        if name not in funcs or depth_at[m.start(1)]:
            continue
        decl = text[m.start(1):m.end(1)]
        if name in declared:
            rep = "/* PORT: this file's own declaration of %s removed: the header declares it, with C linkage (W2). */" % name
        else:
            rep = 'extern "C" ' + re.sub(r"^extern\s+", "", decl) + "   /* PORT: C linkage, as defined (W2) */"
        cuts.append((m.start(1), m.end(1), rep))
    out = text
    for s, e, rep in sorted(cuts, reverse=True):
        out = out[:s] + rep + out[e:]
    return out, len(cuts)


def main(argv):
    # first the definitions: a function some C++ file defines with C++ linkage, that others
    # call by another signature, is given C linkage where it is defined
    _srcs, _failed, undefined, _twice, _ok = und.measure()
    names = und.demangle(undefined)
    bases = {names[s].split("(")[0].strip() for s in undefined
             if s.startswith("__Z") and "::" not in names.get(s, s).split("(")[0]}
    # and C callers' plain names (`_ColChild_CancelMessage`) that a C++ file defines mangled
    bases |= {s[1:] for s in undefined if s.startswith("_") and not s.startswith("__")}
    for p, funcs in sorted(cpp_definitions(bases).items()):
        text = open(p).read()
        new, n = define_c(text, funcs)
        if not n:
            continue
        if "--dry" in argv:
            print("define C: %-40s %s" % (os.path.relpath(p, ROOT), ", ".join(sorted(funcs))))
            continue
        before = ca.compile_one("mingw-i686", p)
        open(p, "w").write(new)
        after = ca.compile_one("mingw-i686", p)
        if not (after["ok"] or dt.error_set(after) <= dt.error_set(before)):
            open(p, "w").write(text)
            print("  definition restored %s: %s" % (os.path.relpath(p, ROOT), (after["first_raw"] or "")[:80]))
    todo = wanted()
    declared = header_cpp_names()
    kept, restored, untouched = 0, [], []
    for p, funcs in sorted(todo.items()):
        text = open(p).read()
        new, n = rewrite(text, funcs, declared)
        if not n:
            untouched.append((p, funcs))
            continue
        if "--dry" in argv:
            print("%-50s %s" % (os.path.relpath(p, ROOT), ", ".join(sorted(funcs))))
            continue
        before = ca.compile_one("mingw-i686", p)
        open(p, "w").write(new)
        after = ca.compile_one("mingw-i686", p)
        if not after["ok"]:
            found = fc.casts(p)
            if found:
                t2, lines = fc.apply(open(p).read(), found)
                open(p, "w").write("/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), "
                                   "lines %s of the imported file. */\n" % ", ".join(map(str, lines)) + t2)
                after = ca.compile_one("mingw-i686", p)
        if after["ok"] or dt.error_set(after) <= dt.error_set(before):
            kept += 1
        else:
            open(p, "w").write(text)
            restored.append((p, after["first_raw"]))
    print("%d files to change; edited %d, restored %d; %d with no file-scope declaration to change" % (
        len(todo), kept, len(restored), len(untouched)))
    for p, why in restored:
        print("  restored %-46s %s" % (os.path.relpath(p, ROOT), (why or "")[:80]))
    for p, funcs in untouched[:20]:
        print("  no declaration: %-40s %s" % (os.path.relpath(p, ROOT), ", ".join(sorted(funcs))))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
