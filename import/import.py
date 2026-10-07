#!/usr/bin/env python3
"""Import the ported corpus from col-win-re at the commit in import/SOURCE.

    python3 import/import.py            # write src/{game,gfx,platform}/ and include/
    python3 import/import.py --check    # change nothing; compare the tree with a fresh import

col-win-re is found at --col-win-re PATH, $COL_WIN_RE, or ../../dderevjanik/col-win-re
from this repository. Its working tree is never read or written: the pinned commit is
unpacked with `git archive` into a temporary directory and its own
tools/port_transform.py runs there, so an import depends on SOURCE alone.

What an import writes (everything else in the tree is this repository's own):
    src/<layer>/<stem>.c   matched/<layer>/<stem>.c through port_transform.py
    include/colonization.h analysis/port-header.h at SOURCE (the generated header)
    include/<name>.h       include/<name>.h at SOURCE (records, classes, constants)
    import/REFUSED         the files port_transform.py would not transform, and why

--check compares a fresh import with the tree, file by file:
    identical   the same bytes
    edited      differs, and the file carries a `PORT:` comment (a deliberate port edit)
    differs     differs with no `PORT:` comment -- an unmarked edit, a failure
    missing     produced by the import, absent here -- a failure
    extra       present in an imported directory, not produced -- a failure
and writes status/import.json for tools/status.py. It exits 0 when nothing fails.

A plain import refuses to run over files with `PORT:` edits: once the port has diverged, a
re-import from a newer SOURCE is a three-way merge (roadmap section 2), never an overwrite.
"""
import json
import os
import re
import shutil
import subprocess
import sys
import tarfile
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LAYERS = ("game", "gfx", "platform")
PORT_MARK = re.compile(r"\bPORT:")


def col_win_re(argv):
    if "--col-win-re" in argv:
        return os.path.abspath(argv[argv.index("--col-win-re") + 1])
    if os.environ.get("COL_WIN_RE"):
        return os.path.abspath(os.environ["COL_WIN_RE"])
    return os.path.abspath(os.path.join(ROOT, "..", "..", "dderevjanik", "col-win-re"))


def produce(cwr, source, tmp):
    """{relative path in this repo: bytes}, and the transform's REFUSED text."""
    if subprocess.run(["git", "-C", cwr, "cat-file", "-e", source + "^{commit}"],
                      capture_output=True).returncode:
        sys.exit("col-win-re at %s does not have commit %s (fetch it, or pass --col-win-re)" % (cwr, source))
    arc = os.path.join(tmp, "src.tar")
    with open(arc, "wb") as f:
        subprocess.run(["git", "-C", cwr, "archive", source, "--", "tools/port_transform.py",
                        "analysis/port-header.h", "matched", "include"], stdout=f, check=True)
    tree = os.path.join(tmp, "tree")
    with tarfile.open(arc) as t:
        t.extractall(tree)
    r = subprocess.run([sys.executable, "tools/port_transform.py"], cwd=tree, capture_output=True, text=True)
    if r.returncode:
        sys.exit("port_transform.py failed:\n" + r.stdout[-2000:] + r.stderr[-2000:])
    out = os.path.join(tree, "analysis", "rebuild", "port-src")
    files = {}
    for layer in LAYERS:
        d = os.path.join(out, layer)
        for name in sorted(os.listdir(d)) if os.path.isdir(d) else []:
            files["src/%s/%s" % (layer, name)] = open(os.path.join(d, name), "rb").read()
    files["include/colonization.h"] = open(os.path.join(out, "colonization.h"), "rb").read()
    for name in sorted(os.listdir(os.path.join(tree, "include"))):
        if name.endswith(".h"):
            files["include/" + name] = open(os.path.join(tree, "include", name), "rb").read()
    # the transform's own report of what it refused, without its counts of the moment
    refused = [l.strip() for l in r.stdout.splitlines()
               if l.startswith("    ") and l.strip()]
    text = ("# Files col-win-re's tools/port_transform.py refused at SOURCE; they are not in src/.\n"
            + "".join(l + "\n" for l in refused))
    files["import/REFUSED"] = text.encode()
    return files


def managed(path):
    """A path an import owns: the transformed layers and the headers it brings."""
    return (any(path.startswith("src/%s/" % l) for l in LAYERS) or
            (path.startswith("include/") and path.endswith(".h") and path.count("/") == 1))


def tree_files():
    out = []
    for base in ["src/%s" % l for l in LAYERS] + ["include"]:
        d = os.path.join(ROOT, base)
        for name in sorted(os.listdir(d)) if os.path.isdir(d) else []:
            p = "%s/%s" % (base, name)
            if os.path.isfile(os.path.join(ROOT, p)) and managed(p):
                out.append(p)
    return out


def check(files, source):
    counts = {"identical": 0, "edited": 0, "differs": 0, "missing": 0, "extra": 0}
    fails = []
    for p, data in sorted(files.items()):
        q = os.path.join(ROOT, p)
        if not os.path.exists(q):
            counts["missing"] += 1
            fails.append("missing  " + p)
            continue
        have = open(q, "rb").read()
        if have == data:
            counts["identical"] += 1
        elif PORT_MARK.search(have.decode("latin-1")):
            counts["edited"] += 1
        else:
            counts["differs"] += 1
            fails.append("differs  " + p + "  (no PORT: comment)")
    for p in tree_files():
        if p not in files:
            counts["extra"] += 1
            fails.append("extra    " + p)
    for f in fails[:40]:
        print("  " + f)
    ok = not fails
    print("import --check at %s: %s" % (source[:10], ", ".join("%d %s" % (v, k) for k, v in counts.items())))
    print("REPRODUCED" if ok else "FAILED: %d file(s)" % len(fails))
    os.makedirs(os.path.join(ROOT, "status"), exist_ok=True)
    srcs = sum(1 for p in files if p.startswith("src/"))
    json.dump({"source": source, "files": len(files), "sources": srcs, "ok": ok, **counts},
              open(os.path.join(ROOT, "status", "import.json"), "w"), indent=1, sort_keys=True)
    open(os.path.join(ROOT, "status", "import.json"), "a").write("\n")
    return 0 if ok else 1


def main(argv):
    source = open(os.path.join(ROOT, "import", "SOURCE")).read().strip()
    if not re.fullmatch(r"[0-9a-f]{40}", source):
        sys.exit("import/SOURCE must hold one full commit hash")
    cwr = col_win_re(argv)
    with tempfile.TemporaryDirectory() as tmp:
        files = produce(cwr, source, tmp)
    if "--check" in argv:
        return check(files, source)
    edited = [p for p in tree_files()
              if PORT_MARK.search(open(os.path.join(ROOT, p), "rb").read().decode("latin-1"))]
    if edited:
        sys.exit("%d imported file(s) carry PORT: edits (%s, ...): a re-import is a three-way merge, "
                 "not an overwrite" % (len(edited), edited[0]))
    for p in tree_files():
        if p not in files:
            os.remove(os.path.join(ROOT, p))
    for p, data in files.items():
        q = os.path.join(ROOT, p)
        os.makedirs(os.path.dirname(q), exist_ok=True)
        open(q, "wb").write(data)
    print("imported %d files from col-win-re %s" % (len(files), source[:10]))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
