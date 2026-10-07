#!/usr/bin/env python3
"""One definition per function (roadmap W2): remove in-file stubs and per-module copies.

    python3 tools/twins.py --dry     # list what would be removed, and what needs a person
    python3 tools/twins.py           # remove it, each with a `PORT:` note; keep edits that compile

The byte-matching corpus defines some functions more than once, for two reasons:
  - STUBS. A file that needed a callee's 4-byte `push cs; call` shape defined the callee
    itself with an empty body (`void set_button_action_1030_e5bf(void *c, void (*fn)()) { }`,
    `Font::Font() { }`). In the port such a stub would BE the function for that file's
    callers -- in a C++ file it even links silently, its mangled name differing from the
    real C one -- so every stub goes.
  - COPIES. Borland compiled inline functions out of line in each module (-vi-), so the
    DialogWin constructor exists thirteen times, at thirteen addresses. The port keeps one.

For every name defined more than once (with the same number of parameters), the definition
kept is the one in the file named for it (`Font_ctor_1008_d478.c` for `Font::Font()`; the
lowest address first), else the first non-trivial one. Each other definition is removed when
it is trivial (a stub: `{ }` or a lone `return`) or the same code as the kept one, with the
per-module copies it calls (`ClearEventTable_1010_3036` for `ClearEventTable_1008_ea88`)
counted as the same. A definition that differs otherwise is listed and left for a person.
A file is edited only if it still compiles, or gains no error it did not have.

Globals: a variable two files define (create_game_globals.c and destroy_game_globals.c are
one source text, compiled once as the static constructor and once as the destructor) keeps
its definition in the first file; the others become `extern` declarations.
"""
import collections
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import compile_all as ca      # noqa: E402
import dedupe_types as dt     # noqa: E402
import fix_casts as fc        # noqa: E402

MODULE_COPY = re.compile(r"\b(\w+?)_10[0-7][0-9a-f]_[0-9a-f]{4}\b")
HEAD = re.compile(r"(?:^|(?<=[;}]))[ \t]*((?:extern\s+\"C\"\s+)?(?:template\s*<\s*>\s*)?"
                  r"(?:[A-Za-z_][\w<>,]*[\s\*&]+)*?"
                  r"((?:\w+(?:<\w+>)?::)?~?\w+)\s*\(([^;{}()]*(?:\([^;{}()]*\)[^;{}()]*)*)\)\s*(?:const\s*)?)\{",
                  re.M)
KEYWORDS = {"if", "while", "for", "switch", "return", "sizeof", "else", "do"}


def blank(text):
    """text with comments and string/char literal contents replaced by spaces (offsets hold)."""
    out = list(text)
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            for k in range(i, j):
                if out[k] != "\n":
                    out[k] = " "
            i = j
        elif text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
            for k in range(i, j):
                out[k] = " "
            i = j
        elif c in "\"'":
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == "\\" else 1
            for k in range(i + 1, min(j, n)):
                if out[k] != "\n":
                    out[k] = " "
            i = j + 1
        else:
            i += 1
    return "".join(out)


def arity(params):
    p = params.strip()
    if p in ("", "void"):
        return 0
    depth, n = 0, 1
    for ch in p:
        if ch in "(<":
            depth += 1
        elif ch in ")>":
            depth -= 1
        elif ch == "," and depth == 0:
            n += 1
    return n


def definitions(path):
    """[(name, arity, start, end, body)] of the file-scope function definitions in path."""
    text = open(path, errors="replace").read()
    b = blank(text)
    out, depth_at = [], []
    # file scope: positions at brace depth 0
    depth = 0
    for i, ch in enumerate(b):
        depth_at.append(depth)
        if ch == "{":
            depth += 1
        elif ch == "}":
            depth -= 1
    for m in HEAD.finditer(b):
        if depth_at[m.start(1)] != 0 or m.group(2) in KEYWORDS:
            continue
        if re.match(r"\s*(class|struct|union|enum|typedef|namespace)\b", m.group(1)):
            continue
        o = m.end() - 1
        d = 0
        for j in range(o, len(b)):
            if b[j] == "{":
                d += 1
            elif b[j] == "}":
                d -= 1
                if d == 0:
                    break
        start = m.start(1)
        out.append((m.group(2), arity(m.group(3)), start, j + 1, b[o:j + 1]))
    return text, out


def norm(body):
    return re.sub(r"\s+", " ", MODULE_COPY.sub(r"\1_COPY", body)).strip()


def trivial(body):
    return re.fullmatch(r"\{\s*(?:return(?:\s+[^;{}]*)?;\s*)?\}", body.strip()) is not None


def stem_of(name):
    """The file stem that defines `name` in the corpus: Class_ctor, Class_dtor, Class_Method, name."""
    if "::" in name:
        cls, meth = name.split("::", 1)
        cls = re.sub(r"<(\w+)>", r"_\1", cls)
        if meth == cls.split("_")[0] or meth == cls:
            return cls + "_ctor"
        if meth.startswith("~"):
            return cls + "_dtor"
        return cls + "_" + meth
    return name


def owns(path, name):
    stem = os.path.splitext(os.path.basename(path))[0]
    want = stem_of(name)
    return stem == want or re.sub(r"_10[0-7][0-9a-f]_[0-9a-f]{4}$", "", stem) == want


def address(path):
    m = re.search(r"_(10[0-7][0-9a-f])_([0-9a-f]{4})\.c$", path)
    return (m.group(1), m.group(2)) if m else ("zzzz", "zzzz")


def plan():
    files = {}
    groups = collections.defaultdict(list)
    for p in ca.sources():
        text, defs = definitions(p)
        files[p] = text
        for name, n, s, e, body in defs:
            groups[(name, n)].append((p, s, e, body))
    removals = collections.defaultdict(list)     # path -> [(start, end, note)]
    review = []
    for (name, n), ds in sorted(groups.items()):
        if len({p for p, *_ in ds}) < 2:
            continue
        owned = sorted([d for d in ds if owns(d[0], name)], key=lambda d: (address(d[0]), d[0]))
        real = [d for d in ds if not trivial(d[3])]
        keep = (owned or sorted(real, key=lambda d: d[0]) or sorted(ds, key=lambda d: d[0]))[0]
        where = os.path.relpath(keep[0], os.path.join(ROOT, "src"))
        for d in ds:
            if d is keep or d[0] == keep[0]:
                continue
            if trivial(d[3]) and not trivial(keep[3]):
                why = "an in-file stub of %s removed: src/%s defines it" % (name, where)
            elif norm(d[3]) == norm(keep[3]):
                why = "this copy of %s removed: src/%s defines it once" % (name, where)
            else:
                review.append((name, n, os.path.relpath(d[0], ROOT), where))
                continue
            removals[d[0]].append((d[1], d[2], why))
    return files, removals, review


def edit(text, cuts):
    out, pos = [], 0
    for s, e, why in sorted(cuts):
        out.append(text[pos:s])
        out.append("/* PORT: %s (W2). */" % why)
        pos = e
    out.append(text[pos:])
    return "".join(out)


def main(argv):
    files, removals, review = plan()
    total = sum(len(v) for v in removals.values())
    print("%d definitions to remove in %d files; %d differ and need a person" % (total, len(removals), len(review)))
    for name, n, p, keep in review:
        print("  review: %s/%d in %s (kept: src/%s)" % (name, n, p, keep))
    if "--dry" in argv:
        for p, cuts in sorted(removals.items()):
            for s, e, why in cuts:
                print("  %-50s %s" % (os.path.relpath(p, ROOT), why))
        return 0
    kept, restored = 0, []
    for p, cuts in sorted(removals.items()):
        before = ca.compile_one("mingw-i686", p)
        open(p, "w").write(edit(files[p], cuts))
        after = ca.compile_one("mingw-i686", p)
        if not after["ok"]:
            # the callers now meet the real declaration: the pointer casts Borland made silently
            found = fc.casts(p)
            if found:
                text, lines = fc.apply(open(p).read(), found)
                open(p, "w").write("/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), "
                                   "lines %s of the imported file. */\n" % ", ".join(map(str, lines)) + text)
                after = ca.compile_one("mingw-i686", p)
        if after["ok"] or dt.error_set(after) <= dt.error_set(before):
            kept += 1
        else:
            open(p, "w").write(files[p])
            restored.append((p, after["first_raw"]))
    print("edited %d files; restored %d" % (kept, len(restored)))
    for p, why in restored:
        print("  restored %-48s %s" % (os.path.relpath(p, ROOT), (why or "")[:80]))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
