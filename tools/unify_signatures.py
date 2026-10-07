#!/usr/bin/env python3
"""One signature per function: its definition's (roadmap W1 step 3).

    python3 tools/unify_signatures.py

The byte-matching corpus let each file declare its callees as it liked -- `void *` for a
pointer the definition types as `SScrollBar *`, `char *` for `unsigned char *` -- because a
16-bit compiler pushes the same bytes for both. The port has one declaration per function,
in include/gen/colonization.h, and it is the definition's: the definition is what compiled to
the shipped code. So a file's own declaration of a function src/ defines, spelled another way,
is removed (with a `PORT:` note naming it), the header then declares the function from its
definition (tools/gen_headers.py), and the callers that passed another pointer type get an
explicit cast (tools/fix_casts.py).

It works by trial, from the files as they were each time: remove the declarations of a set of
functions, regenerate, cast, compile everything. A file that compiled before and does not now
gives up every function of the set it declared or names, and the trial runs again without them,
until no file that compiled is lost.
"""
import os
import re
import subprocess
import sys
import concurrent.futures

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import compile_all as ca      # noqa: E402
import gen_headers as gh      # noqa: E402

DECL = re.compile(r"^[ \t]*((?:extern\s+(?:\"C\"\s+)?)?[A-Za-z_][^;{}()#=]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*;)[ \t]*\n?", re.M)


def conflicts():
    heads = gh.definition_heads()
    _, own = gh.file_declarations()
    return {n for n, s in own.items() if n in heads and s != {gh.signature(heads[n] + ";")}}, heads


def remove_decls(text, names, heads):
    """text without its file-scope declarations of `names` that differ from the definition."""
    scope = gh.file_scope(text)
    cut, removed = [], []
    for m in DECL.finditer(scope):
        n = m.group(2)
        if n in names and gh.signature(m.group(1)) != gh.signature(heads[n] + ";"):
            cut.append((m.start(), m.end()))
            removed.append(n)
    if not cut:
        return text, []
    out, pos = [], 0
    for a, b in cut:
        out.append(text[pos:a])
        pos = b
    out.append(text[pos:])
    note = ("/* PORT: this file's own declarations of %s removed: the header declares %s from "
            "the definition (W1 step 3). */\n" % (", ".join(sorted(set(removed))),
                                                 "it" if len(set(removed)) == 1 else "them"))
    return note + "".join(out), sorted(set(removed))


def compile_set():
    with concurrent.futures.ThreadPoolExecutor(os.cpu_count() or 4) as ex:
        res = list(ex.map(lambda p: ca.compile_one("mingw-i686", p), ca.sources()))
    return {r["path"] for r in res if r["ok"]}


def main(argv):
    names, heads = conflicts()
    paths = ca.sources()
    original = {p: open(p).read() for p in paths}
    base = compile_set()
    print("%d functions declared another way; %d files compile before" % (len(names), len(base)))
    trial = set(names)
    for rnd in range(1, 12):
        for p in paths:
            open(p, "w").write(original[p])
        touched = {}
        for p in paths:
            new, removed = remove_decls(original[p], trial, heads)
            if removed:
                open(p, "w").write(new)
                touched[os.path.relpath(p, ROOT)] = removed
        subprocess.run([sys.executable, os.path.join(ROOT, "tools", "gen_headers.py")], capture_output=True)
        subprocess.run([sys.executable, os.path.join(ROOT, "tools", "fix_casts.py")], capture_output=True)
        now = compile_set()
        lost = base - now
        print("round %d: %d functions, %d files edited, %d compile (%d lost, %d gained)" % (
            rnd, len(trial), len(touched), len(now), len(lost), len(now - base)))
        if not lost:
            break
        give_up = set()
        for p in lost:
            text = open(os.path.join(ROOT, p)).read()
            give_up |= set(touched.get(p, ())) | {n for n in trial if re.search(r"\b%s\b" % re.escape(n), text)}
        trial -= give_up
    else:
        for p in paths:
            open(p, "w").write(original[p])
        subprocess.run([sys.executable, os.path.join(ROOT, "tools", "gen_headers.py")], capture_output=True)
        print("no settled set: everything restored")
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
