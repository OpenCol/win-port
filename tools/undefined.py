#!/usr/bin/env python3
"""What the link still lacks (roadmap W2 gate).

    python3 tools/undefined.py              # build objects, report what is undefined or twice defined
    python3 tools/undefined.py --list       # every undefined symbol, with the objects that want it
    python3 tools/undefined.py --record     # also write status/link.json (tools/status.py reads it)

Every file in src/ is compiled to an object (MinGW i686, -O0; objects are cached in
build/obj by modification time, and a header change rebuilds all). `nm` gives each object's
defined and undefined symbols. A symbol is
  - resolved when some object defines it;
  - undefined when none does -- grouped by what will supply it: a Win16 API name the
    platform layer (W4) maps to Win32, the assembly segments' routines (src/asm, W5), the
    C runtime, or nothing yet;
  - twice defined when two objects define it (a stub, or a definition the corpus repeats).
The gate (W2): no undefined symbol outside the named backlogs (Win16 API for W4, assembly for
W5), none twice defined, and the executable links once each backlog symbol is trapped by a
generated stub (build/backlog.c) that names the function and stops. status/link.json lists
the backlog by name; its size is the milestone's number, down to 0 at W4 and W5.
"""
import collections
import concurrent.futures
import glob
import json
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import compile_all as ca      # noqa: E402

OBJ = os.path.join(ROOT, "build", "obj")
NM = "i686-w64-mingw32-nm"
CC, CXX, _ = ca.TOOLCHAINS["mingw-i686"]


def headers_mtime():
    return max(os.path.getmtime(p) for p in glob.glob(os.path.join(ROOT, "include", "**", "*.h"), recursive=True))


def obj_path(src):
    rel = os.path.relpath(src, os.path.join(ROOT, "src"))
    return os.path.join(OBJ, os.path.splitext(rel)[0] + ".o")


def build(src, hdr):
    o = obj_path(src)
    if os.path.exists(o) and os.path.getmtime(o) > max(os.path.getmtime(src), hdr):
        return o, None
    os.makedirs(os.path.dirname(o), exist_ok=True)
    cpp = ca.is_cpp(src)
    cmd = ([CXX, "-x", "c++", "-std=gnu++03"] if cpp else [CC, "-x", "c", "-std=gnu99"]) + [
        "-c", "-O0", "-w", "-fno-asynchronous-unwind-tables",
        "-I", os.path.join(ROOT, "include", "gen"), "-I", os.path.join(ROOT, "include"), "-o", o, src]
    r = subprocess.run(cmd, capture_output=True, text=True, errors="replace", stdin=subprocess.DEVNULL, timeout=300)
    if r.returncode:
        if os.path.exists(o):
            os.remove(o)
        return None, r.stderr
    return o, None


LIBS = ["-mwindows", "-lgdi32", "-luser32", "-lwinmm", "-lkernel32", "-lcomdlg32"]
UNDEF = re.compile(r"undefined reference to `([^']+)'")
TWICE = re.compile(r"multiple definition of `([^']+)'")


def sources():
    """Every file the executable is linked from: src/ as imported, src/gen (generated) and
    src/host (the port's own runtime and glue; its self-test is a program of its own)."""
    host = [p for p in glob.glob(os.path.join(ROOT, "src", "host", "*.c")) if not p.endswith("selftest.c")]
    return ca.sources() + sorted(glob.glob(os.path.join(ROOT, "src", "gen", "*.c"))) + sorted(host)


def link(objs):
    """{symbol: [objects that want it]}, {symbol: [objects that define it]} from the linker."""
    lst = os.path.join(ROOT, "build", "link.rsp")
    with open(lst, "w") as f:
        f.write("\n".join(objs) + "\n")
    exe = os.path.join(ROOT, "build", "colonize.exe")
    r = subprocess.run([CXX, "-o", exe, "@" + lst, "-Wl,--no-demangle"] + LIBS,
                       capture_output=True, text=True, errors="replace")
    undefined, twice = collections.defaultdict(set), collections.defaultdict(set)
    link.stderr = r.stderr
    for line in r.stderr.splitlines():
        o = re.search(r"(\S+?\.o)\b", line)        # ld: build/obj/game/x.o:x.c:(.text+0x1d): ...
        src = os.path.relpath(o.group(1), OBJ)[:-2] + ".c" if o else "?"
        m = UNDEF.search(line)
        if m:
            undefined[m.group(1)].add(src)
        m = TWICE.search(line)
        if m:
            twice[m.group(1)].add(src)
    return undefined, twice, r.returncode == 0


def demangle(names):
    names = list(names)
    if not names:
        return {}
    r = subprocess.run(["i686-w64-mingw32-c++filt", "-_"], input="\n".join(names), capture_output=True, text=True)
    return dict(zip(names, r.stdout.splitlines()))


def win16_names():
    imp = json.load(open(os.path.join(ROOT, "import", "win16-imports.json")))
    return {n.upper() for names in imp.values() for n in names}


def classify(sym, demangled, api):
    """The backlog an undefined symbol belongs to."""
    plain = sym[1:] if sym.startswith("_") else sym
    if plain.upper() in api or plain.upper().lstrip("_") in api:
        return "win16 API (W4: src/platform maps it to Win32)"
    if plain.startswith("asm_") or re.match(r"L1[789]_[0-9a-f]{4}$", plain):
        return "assembly (W5: src/asm)"
    if plain.startswith("g_"):
        return "global without storage"
    if sym.startswith("__Z") or sym.startswith("_Z"):
        return "C++ name (a declaration's linkage or signature differs from the definition's)"
    return "function nothing defines"


BACKLOGS = ("win16 API (W4: src/platform maps it to Win32)", "assembly (W5: src/asm)")


def trap_link(objs, names):
    """Link again with a trap for each backlog symbol (build/backlog.c, not committed): the
    executable exists, and calling a function not yet ported says which and stops."""
    src = os.path.join(ROOT, "build", "backlog.c")
    lines = ["/* GENERATED by tools/undefined.py: the W4/W5 backlog, trapped. Not committed. */",
             "#include <stdio.h>", "#include <stdlib.h>",
             "static void unported(const char *name) { fprintf(stderr, \"not ported yet: %s\\n\", name); abort(); }"]
    for sym in sorted(names):
        c = sym[1:] if sym.startswith("_") else sym
        lines.append("void %s(void) { unported(\"%s\"); }" % (c, c))
    with open(src, "w") as f:
        f.write("\n".join(lines) + "\n")
    obj = os.path.join(ROOT, "build", "backlog.o")
    r = subprocess.run([CC, "-c", "-w", "-o", obj, src], capture_output=True, text=True)
    if r.returncode:
        return False, r.stderr
    undefined, twice, ok = link(objs + [obj])
    if ok and not undefined and not twice:
        return True, ""
    why = ["undefined " + n for n in sorted(undefined)[:10]] + ["twice " + n for n in sorted(twice)[:10]]
    if not why:
        why = [l for l in link.stderr.splitlines() if "error" in l or "ld:" in l][:10]
    return False, "; ".join(why)


def measure():
    srcs = sources()
    hdr = headers_mtime()
    with concurrent.futures.ThreadPoolExecutor(os.cpu_count() or 4) as ex:
        built = list(ex.map(lambda s: build(s, hdr), srcs))
    failed = [(s, err) for s, (o, err) in zip(srcs, built) if o is None]
    objs = [o for o, _e in built if o]
    undefined, twice, ok = link(objs)
    measure.objs = objs
    return srcs, failed, undefined, twice, ok


def main(argv):
    srcs, failed, undefined, twice, ok = measure()
    names = demangle(set(undefined) | set(twice))
    api = win16_names()
    print("objects: %d of %d built" % (len(srcs) - len(failed), len(srcs)))
    for s, err in failed[:10]:
        first = next((l for l in err.splitlines() if "error" in l), err[:200])
        print("  not built: %s: %s" % (os.path.relpath(s, ROOT), first[:150]))
    groups = collections.defaultdict(list)
    for n in undefined:
        groups[classify(n, names.get(n, n), api)].append(n)
    print("link: %s; undefined: %d symbols; defined twice: %d" % (
        "ok" if ok else "fails", len(undefined), len(twice)))
    for g, ns in sorted(groups.items(), key=lambda kv: -len(kv[1])):
        print("  %4d  %s" % (len(ns), g))
        if "--list" in argv:
            for n in sorted(ns, key=lambda n: (-len(undefined[n]), n)):
                w = sorted(undefined[n])
                print("          %-50s %4d  e.g. %s" % (names.get(n, n)[:50], len(w), w[0]))
    if twice:
        print("  %4d  defined twice" % len(twice))
        if "--list" in argv:
            for n in sorted(twice):
                print("          %-50s %s" % (names.get(n, n)[:50], ", ".join(sorted(twice[n])[:4])))
    # the gate: nothing undefined outside the named backlogs, nothing twice, and with the
    # backlog trapped the executable links
    outside = {g: ns for g, ns in groups.items() if g not in BACKLOGS}
    gate = not failed and not twice and not outside
    linked = False
    if gate:
        linked, err = trap_link(measure.objs, [n for g in BACKLOGS for n in groups.get(g, [])])
        print("with the backlog trapped: %s" % ("build/colonize.exe links" if linked else "still fails " + err[:200]))
    ok = gate and linked
    if "--record" in argv:
        out = {"objects": len(srcs) - len(failed), "sources": len(srcs), "links": linked, "gate": ok,
               "undefined": len(undefined), "twice": len(twice),
               "outside_backlog": sorted(n for ns in outside.values() for n in ns),
               "backlog": {g: sorted(n[1:] if n.startswith("_") else n for n in groups.get(g, []))
                           for g in BACKLOGS}}
        with open(os.path.join(ROOT, "status", "link.json"), "w") as fh:
            json.dump(out, fh, indent=1, sort_keys=True)
            fh.write("\n")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
