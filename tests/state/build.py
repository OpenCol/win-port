#!/usr/bin/env python3
"""Build build/port_state.exe, the native side of tests/state.py (roadmap W3).

    python3 tests/state/build.py

The harness (tests/state/harness.c) is linked with every game object -- the same objects
tools/undefined.py links, the W4/W5 backlog trapped -- so its globals ARE the game's. Its
table (build/state/table.c, generated here) lists each global the model addresses and the
port declares: the address, one element's native size (`sizeof`, from the port's own
headers, so it is whatever the compiler laid out) and the element count -- the declared
one, or, for an open array, the game's (the extent tools/gen_globals.py gives it).

An entry the compiler rejects (a type only one language knows, an incomplete view) is
dropped and listed; it is reported, never silently absent. The image base is fixed
(--disable-dynamicbase) so the addresses `layout` prints hold for every run.
"""
import json
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import gen_globals as gg      # noqa: E402
import undefined as und       # noqa: E402

OUT = os.path.join(ROOT, "build", "state")
EXE = os.path.join(ROOT, "build", "port_state.exe")


def entries():
    """[(name, declaration)] of the modelled globals the port declares."""
    layout = json.load(open(os.path.join(ROOT, "import", "layout.json")))
    decls = gg.declarations(cpp="all")      # the table is C++: what a C++ file sees
    extra = {n: d for n, d in gg.file_only().items() if n not in decls}
    addressed = set(layout["globals"]) | set(json.load(open(os.path.join(ROOT, "import", "addresses.json"))))
    # a global C++ cannot see the type of (a C-only view: g_main_win) is entered by its
    # address alone, so a pointer to its start still maps
    out = [(n, decls.get(n) or extra.get(n) or 'extern "C" char %s;' % n,
            n in extra or not (decls.get(n) or extra.get(n))) for n in sorted(addressed)]
    return out, layout


def game_count(name, decl, layout, ext):
    m = gg.VAR.match(decl)
    if not m:
        return None
    base, stars, dims = m.group(1), (m.group(2) or "").replace(" ", ""), m.group(4).replace(" ", "")
    inner = re.findall(r"\[([^\]]*)\]", dims)
    if not inner or inner[0].strip():
        return None
    w = gg.game_width(base, stars, layout["records"])
    if not w or name not in ext:
        return None
    for x in inner[1:]:
        w *= int(x, 0)
    return max(1, ext[name] // w)


def classes():
    """{class: (bases, [(type, name, dims)], polymorphic own)} from include/gen/classes.h (C++)."""
    t = open(os.path.join(ROOT, "include", "gen", "classes.h")).read()
    t = re.sub(r"/\*.*?\*/", " ", t, flags=re.S)
    t = t[t.index("#ifdef __cplusplus"):t.index("#else", t.index("#ifdef __cplusplus"))]
    out = {}
    for m in re.finditer(r"\bclass\s+(\w+)\s*(?::\s*([^{]*))?\{(.*?)\};", t, re.S):
        name, bases, body = m.group(1), m.group(2) or "", m.group(3)
        bl = [re.sub(r"\b(public|private|protected|virtual)\b", "", b).strip() for b in bases.split(",") if b.strip()]
        body = re.sub(r"\b(public|private|protected)\s*:", "", body)
        members, poly = [], "virtual" in body
        for stmt in body.split(";"):
            stmt = " ".join(stmt.split())
            if not stmt or "(" in stmt or stmt.startswith(("typedef", "enum", "friend", "static")):
                continue
            mt = re.match(r"((?:unsigned |signed |const )*[A-Za-z_]\w*(?:<\w+>)?)\s+(.*)", stmt)
            if not mt:
                continue
            base_t = mt.group(1)
            for decl in mt.group(2).split(","):
                dm = re.match(r"\s*((?:far |huge |near )?\**\s*(?:far |huge |near )?)\s*(\w+)\s*((?:\[[^\]]*\])*)", decl.strip())
                if dm:
                    members.append((base_t + (" *" if "*" in dm.group(1) else ""), dm.group(2), dm.group(3)))
        out[name] = (bl, members, poly)
    # and the records (records.h): a record with pointer members needs them translated too
    r = re.sub(r"/\*.*?\*/", " ", open(os.path.join(ROOT, "include", "gen", "records.h")).read(), flags=re.S)
    for m in re.finditer(r"typedef\s+struct\s*\w*\s*\{([^{}]*)\}\s*(\w+)\s*;", r):
        body, name = m.group(1), m.group(2)
        if name in out:
            continue
        members = []
        for stmt in body.split(";"):
            stmt = " ".join(stmt.split())
            mt = re.match(r"((?:unsigned |signed |const )*[A-Za-z_]\w*)\s+(.*)", stmt)
            if not mt or "(" in stmt:
                continue
            for decl in mt.group(2).split(","):
                dm = re.match(r"\s*((?:far |huge |near )?\**\s*(?:far |huge |near )?)\s*(\w+)\s*((?:\[[^\]]*\])*)", decl.strip())
                if dm:
                    members.append((mt.group(1) + (" *" if "*" in dm.group(1) else ""), dm.group(2), dm.group(3)))
        out[name] = ([], members, False)
    return out


def diff_functions():
    import importlib.util
    spec = importlib.util.spec_from_file_location("diff", os.path.join(ROOT, "tests", "diff.py"))
    d = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(d)
    return list(d.FUNCTIONS)


def needed_classes(es, cls):
    want, todo = set(), []
    for _n, decl, _x in es:
        m = gg.VAR.match(decl)
        if m and m.group(1).strip() in cls and not (m.group(2) or "").strip():
            todo.append(m.group(1).strip())
    while todo:
        c = todo.pop()
        if c in want:
            continue
        want.add(c)
        bases, members, _p = cls[c]
        todo += [b for b in bases if b in cls]
        todo += [t for t, _n, _d in members if t in cls]
    return sorted(want)


def fields(names, cls):
    """C++ rows: each class's size, its bases' offsets, its own members' offsets and sizes."""
    rows = []
    for c in names:
        bases, members, _p = cls[c]
        rows.append('  { "%s", "size", "", sizeof(%s), 0 },' % (c, c))
        for b in bases:
            rows.append('  { "%s", "base", "%s", (unsigned)((char *)(%s *)(%s *)0x1000 - (char *)0x1000), sizeof(%s) },'
                        % (c, b, b, c, b))
        for t, n, dims in members:
            rows.append('  { "%s", "member", "%s", (unsigned)__builtin_offsetof(%s, %s), sizeof(((%s *)0)->%s) },'
                        % (c, n, c, n, c, n))
    return rows


def table(es, layout, drop):
    ext = gg.extents(layout)
    lines = ["/* GENERATED by tests/state/build.py: the modelled globals, natively. */",
             '#include "colonization.h"', '#include "classes.h"', ""]
    for name, decl, extra in es:
        if extra and name not in drop:
            lines.append(decl)
    lines += ["", 'extern "C" {', "struct PortGlobal { const char *name; void *addr; unsigned elem; unsigned count; };",
              "extern const struct PortGlobal port_globals[];", "extern const unsigned port_global_count;",
              "const struct PortGlobal port_globals[] = {"]
    where = {}
    n = 0
    for name, decl, _extra in es:
        if name in drop:
            continue
        m = gg.VAR.match(decl)
        dims = m.group(4).strip() if m else ""
        if decl.startswith('extern "C" char '):
            row = '  { "%s", (void *)&%s, 0, 0 },' % (name, name)      # by address only
        elif dims:
            cnt = game_count(name, decl, layout, ext)
            count = str(cnt) if cnt else "sizeof(%s) / sizeof(%s[0])" % (name, name)
            row = '  { "%s", (void *)&%s, sizeof(%s[0]), %s },' % (name, name, name, count)
        else:
            row = '  { "%s", (void *)&%s, sizeof(%s), 1 },' % (name, name, name)
        lines.append(row)
        where[len(lines)] = name
        n += 1
    lines += ["};", "const unsigned port_global_count = %d;" % n]
    cls = classes()
    lines += ["struct PortField { const char *cls; const char *what; const char *name; unsigned off; unsigned size; };",
              "extern const struct PortField port_fields[];", "extern const unsigned port_field_count;",
              "const struct PortField port_fields[] = {"]
    rows = fields(needed_classes([e for e in es if e[0] not in drop], cls), cls)
    lines += rows + ["};", "const unsigned port_field_count = %d;" % len(rows)]
    # the functions the differential calls (tests/diff.py), by name
    fns = diff_functions()
    lines += ["struct PortFunction { const char *name; void (*fn)(void); };",
              "extern const struct PortFunction port_functions[];", "extern const unsigned port_function_count;",
              "const struct PortFunction port_functions[] = {"]
    lines += ['  { "%s", (void (*)(void))%s },' % (f, f) for f in fns] or ['  { "", 0 },']
    lines += ["};", "const unsigned port_function_count = %d;" % len(fns), "}"]
    return "\n".join(lines) + "\n", where


def main(argv):
    os.makedirs(OUT, exist_ok=True)
    es, layout = entries()
    drop = {}
    src = os.path.join(OUT, "table.c")
    obj = os.path.join(OUT, "table.o")
    inc = ["-I", os.path.join(ROOT, "include", "gen"), "-I", os.path.join(ROOT, "include")]
    for _round in range(8):
        text, where = table(es, layout, drop)
        open(src, "w").write(text)
        r = subprocess.run([und.CXX, "-x", "c++", "-std=gnu++03", "-c", "-w", "-o", obj] + inc + [src],
                           capture_output=True, text=True)
        if r.returncode == 0:
            break
        bad = {}
        for m in re.finditer(r"table\.c:(\d+):\d+: error: (.*)", r.stderr):
            line = int(m.group(1))
            name = where.get(line)
            if name is None:      # an extra declaration line: find the global it names
                t = text.splitlines()[line - 1]
                name = next((n for n, d, x in es if x and d == t.strip()), None)
            if name:
                bad.setdefault(name, m.group(2))
        if not bad:
            sys.exit("table.c does not compile:\n" + r.stderr[-2000:])
        # a declaration C++ cannot compile here enters by its address alone; only one that
        # fails even so is dropped
        again = []
        for name, why in bad.items():
            i = next(k for k, e in enumerate(es) if e[0] == name)
            if es[i][1].startswith('extern "C" char '):
                drop[name] = why
            else:
                es[i] = (name, 'extern "C" char %s;' % name, True)
                again.append(name)
    else:
        sys.exit("table.c still does not compile")
    h_obj = os.path.join(OUT, "harness.o")
    r = subprocess.run([und.CC, "-c", "-w", "-o", h_obj, os.path.join(ROOT, "tests", "state", "harness.c")],
                       capture_output=True, text=True)
    if r.returncode:
        sys.exit(r.stderr)
    # the game's objects and the backlog's traps, as tools/undefined.py links them
    _s, failed, undefined, twice, _ok = und.measure()
    if failed or twice:
        sys.exit("the game objects do not build cleanly: run tools/undefined.py")
    names = [n for n in undefined]
    ok, err = und.trap_link(und.measure.objs, names)
    if not ok:
        sys.exit("the game does not link: " + err)
    # the harness's own backlog: the game's static constructors run before main and call
    # Win16 functions not ported yet (W4); here each says so once and returns 0, since the
    # state the harness loads overwrites whatever they did
    lenient = os.path.join(OUT, "backlog_lenient.c")
    lines = ["/* GENERATED by tests/state/build.py: the W4/W5 backlog, lenient (harness only). */",
             "#include <stdio.h>", "#include <string.h>",
             "static int unported(const char *name) { static char seen[65536]; static unsigned n;",
             "  if (!strstr(seen, name) && n + strlen(name) + 2 < sizeof seen) {",
             "    fprintf(stderr, \"unported (ignored): %s\\n\", name); strcat(seen, name); strcat(seen, \";\"); n = strlen(seen); }",
             "  return 0; }"]
    for sym in sorted(names):
        c = sym[1:] if sym.startswith("_") else sym
        lines.append("int %s(void) { return unported(\"%s\"); }" % (c, c))
    open(lenient, "w").write("\n".join(lines) + "\n")
    l_obj = os.path.join(OUT, "backlog_lenient.o")
    r = subprocess.run([und.CC, "-c", "-w", "-o", l_obj, lenient], capture_output=True, text=True)
    if r.returncode:
        sys.exit(r.stderr)
    objs = [o for o in und.measure.objs] + [l_obj, obj, h_obj]
    rsp = os.path.join(OUT, "link.rsp")
    open(rsp, "w").write("\n".join(objs) + "\n")
    libs = [l for l in und.LIBS if l != "-mwindows"]
    r = subprocess.run([und.CXX, "-o", EXE, "@" + rsp, "-mconsole", "-Wl,--disable-dynamicbase",
                        ] + libs, capture_output=True, text=True)
    if r.returncode:
        sys.exit("link failed:\n" + "\n".join(r.stderr.splitlines()[-15:]))
    print("build/port_state.exe: %d globals; %d dropped" % (len(es) - len(drop), len(drop)))
    for n, why in sorted(drop.items()):
        print("  dropped %-30s %s" % (n, why[:90]))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
