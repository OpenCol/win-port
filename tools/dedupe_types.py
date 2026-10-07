#!/usr/bin/env python3
"""Remove a file's own copy of a record or class the shared headers define (roadmap W1 step 2).

    python3 tools/dedupe_types.py            # edit src/, keeping each edit that helps
    python3 tools/dedupe_types.py --dry      # list what would be removed

Some reconstructions still declare a class or record themselves -- a `class GRPort {...};`
or a C view `typedef struct {...} DialogWin;` -- because the byte-matching build let each
file compile alone. In the port one definition must serve every file: the local copy
contradicts the shared header's (a forward `class GRPort;` cannot meet a typedef of an
anonymous struct), and its Borland `int` members have the wrong width anyway.

For each file this removes the local definitions of names include/gen's records.h or
classes.h define, includes those headers where the first one was, and marks the place with
a `PORT:` comment (CLAUDE.md rule 5). The file is compiled before and after (MinGW i686):
the edit is kept when the file now compiles, or adds no error it did not have; otherwise
the file is restored and listed, because a local copy whose members the shared class does
not have needs a person to read both.
"""
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import compile_all as ca      # noqa: E402
import gen_headers as gh      # noqa: E402


def shared_names():
    cls = gh.strip_comments(open(os.path.join(ROOT, "include", "gen", "classes.h")).read())
    split = cls.index("#else")
    classes = set(re.findall(r"\bclass\s+(\w+)\s*(?::[^{;]*)?\{", cls[:split]))
    views = gh.defined_types(cls[split:])
    records = gh.defined_types(open(os.path.join(ROOT, "include", "records.h")).read())
    # what classes.h gives each language: C++ the classes, C the views
    return {True: classes, False: views}, records


def definitions(text):
    """[(start, end, name)] of file-scope `typedef struct ... {...} Name;`, `struct Name {...};`
    and `class Name ... {...};` statements (comments blanked so offsets hold)."""
    code = gh.strip_comments(text)
    out = []
    # a statement may open its line or follow another on it (`class DlgControl; class DialogWin : ... {`)
    for m in re.finditer(r"(?:^|(?<=;))[ \t]*(typedef\s+struct\s*\w*\s*\{|class\s+(\w+)\s*(?::[^{;]*)?\{|struct\s+(\w+)\s*\{)",
                         code, re.M):
        if out and m.start() < out[-1][1]:
            continue
        o = code.index("{", m.start())
        d = 0
        for j in range(o, len(code)):
            if code[j] == "{":
                d += 1
            elif code[j] == "}":
                d -= 1
                if d == 0:
                    break
        e = code.find(";", j)
        if e < 0:
            continue
        name = m.group(2) or m.group(3) or (re.match(r"\}\s*(\w+)", code[j:e + 1]) or [None, None])[1]
        if name:
            out.append((m.start(), e + 1, name))
    return out


def rewrite(text, cls_names, rec_names):
    defs = [(s, e, n) for s, e, n in definitions(text) if n in cls_names | rec_names]
    if not defs:
        return None, []
    removed = [n for _s, _e, n in defs]
    need = []
    if set(removed) & cls_names and '#include "classes.h"' not in text:
        need.append('#include "classes.h"')
    if set(removed) & rec_names and '#include "records.h"' not in text and '#include "classes.h"' not in text:
        need.append('#include "records.h"')
    note = "/* PORT: this file's own %s removed: include/gen/%s define%s %s once (W1 step 2). */" % (
        ", ".join(removed), " and ".join(sorted({"classes.h" if n in cls_names else "records.h" for n in removed})),
        "" if len({n in cls_names for n in removed}) == 1 else "",
        "it" if len(removed) == 1 else "them")
    out, pos = [], 0
    for k, (s, e, n) in enumerate(defs):
        out.append(text[pos:s])
        if k == 0:
            out.append("\n".join([note] + need))
        pos = e
    out.append(text[pos:])
    return "".join(out), removed


def errors(r):
    return sum(1 for l in r["log"].splitlines() if ": error:" in l)


def error_set(r):
    """The file's error messages, without line numbers: an edit that adds none is kept, since
    removing the local copy is what lets the shared header type the globals right."""
    return {l.split(": error: ", 1)[1] for l in r["log"].splitlines() if ": error: " in l}


def main(argv):
    cls_names, rec_names = shared_names()
    kept, restored, unchanged = [], [], 0
    for p in ca.sources():
        text = open(p).read()
        new, removed = rewrite(text, cls_names[ca.is_cpp(p)], rec_names)
        if new is None:
            unchanged += 1
            continue
        if "--dry" in argv:
            print("%-48s %s" % (os.path.relpath(p, ROOT), ", ".join(removed)))
            continue
        before = ca.compile_one("mingw-i686", p)
        open(p, "w").write(new)
        after = ca.compile_one("mingw-i686", p)
        if after["ok"] or error_set(after) <= error_set(before):
            kept.append((p, removed, after["ok"]))
        else:
            open(p, "w").write(text)
            restored.append((p, removed, after["first_raw"]))
    if "--dry" in argv:
        return 0
    print("edited %d files (%d now compile); restored %d" % (
        len(kept), sum(1 for k in kept if k[2]), len(restored)))
    for p, removed, why in restored:
        print("  restored %-44s %s: %s" % (os.path.relpath(p, ROOT), ",".join(removed)[:40], (why or "")[:70]))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
