#!/usr/bin/env python3
"""Write STATUS.md from what the gates recorded in status/*.json. Never edit STATUS.md by hand.

    python3 tools/status.py            # rewrite STATUS.md
    python3 tools/status.py --check    # fail if STATUS.md is not what the records say (CI)

Every figure on the page comes from a file a gate command wrote, and every row names that
command. A milestone with no record shows "not measured": nothing is estimated. Writers:
    status/import.json   python3 import/import.py --check
    status/ci.json       python3 tools/ci_record.py        (the latest CI run on GitHub)
    status/compile-<toolchain>.json   python3 tools/compile_all.py --record
    status/link.json     python3 tools/undefined.py --record
    status/state.json    python3 tests/state.py --record          (needs $COL_GAME_DIR and wine)
    status/diff.json     python3 tests/diff.py --record           (needs $COL_GAME_DIR and wine)
    status/oracle.json   python3 tests/oracle.py --selftest --record   (needs $COL_GAME_DIR)
    status/trace.json    python3 tests/trace.py --gate --record    (needs $COL_GAME_DIR and wine)
Later milestones add theirs (compile.json, link.json, ...) as their gates are written.
"""
import json
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
STATUS = os.path.join(ROOT, "STATUS.md")


def rec(name):
    p = os.path.join(ROOT, "status", name + ".json")
    return json.load(open(p)) if os.path.exists(p) else None


def w0():
    imp, ci = rec("import"), rec("ci")
    parts, done = [], True
    if imp is None:
        parts.append("import: not measured")
        done = False
    else:
        parts.append("import %s at %s (%d files, %d sources; %d edited by PORT:)" % (
            "reproducible" if imp["ok"] else "NOT reproducible", imp["source"][:10],
            imp["files"], imp["sources"], imp["edited"]))
        done &= imp["ok"]
    if ci is None:
        parts.append("CI: not recorded")
        done = False
    else:
        parts.append("CI %s at %s (run %s)" % (ci["conclusion"], ci["commit"][:7], ci["run"]))
        done &= ci["conclusion"] == "success"
    return ("DONE" if done else "open"), "; ".join(parts)


def w1():
    recs = [r for r in (rec("compile-mingw-i686"), rec("compile-winegcc")) if r]
    if not recs:
        return "--", "not measured"
    parts = ["%s %d/%d" % (r["toolchain"], r["ok"], r["total"]) for r in recs]
    done = len(recs) == 2 and all(r["ok"] == r["total"] for r in recs)
    if recs[0]["first_errors"]:
        n, msg = recs[0]["first_errors"][0]
        parts.append("largest group: %d x %s" % (n, msg.replace("|", "\\|")[:60]))
    return ("DONE" if done else "open"), "; ".join(parts)


def w2():
    r = rec("link")
    if r is None:
        return "--", "not measured"
    bl = r["backlog"]
    parts = ["objects %d/%d" % (r["objects"], r["sources"]),
             "executable %s" % ("links (backlog trapped)" if r["links"] else "does not link"),
             "undefined %d: %s" % (r["undefined"], ", ".join("%d %s" % (len(v), k.split(" (")[0]) for k, v in bl.items())),
             "twice defined %d" % r["twice"]]
    if r["outside_backlog"]:
        parts.append("%d undefined outside the backlogs" % len(r["outside_backlog"]))
    return ("DONE" if r["gate"] else "open"), "; ".join(parts)


def w3():
    parts, done = [], True
    o = rec("oracle")
    if o:
        parts.append("oracle self-test %s (%d turns, %d checkpoints)" % ("identical" if o["ok"] else "DIFFERS",
                                                                      o["turns"], o["checkpoints"]))
        done &= o["ok"]
    else:
        parts.append("oracle: not measured")
        done = False
    st = rec("state")
    if st:
        parts.append("state round trip %s: %d/%d globals converted, %d lost, %d by address only" % (
            "lossless" if st["ok"] else "LOSSY", st["converted"], st["globals"], st["lost"], len(st["address_only"])))
        done &= st["ok"]
    else:
        parts.append("state: not measured")
        done = False
    d = rec("diff")
    if d:
        parts.append("differential %d/%d functions agree" % (d["agree"], d["total"]))
        done &= d["agree"] >= 10 and d["agree"] == d["total"]
    else:
        parts.append("differential: not measured")
        done = False
    return ("DONE" if done else "open"), "; ".join(parts)


def w4():
    link, tr = rec("link"), rec("trace")
    parts, done = [], True
    if link:
        left = len(link["backlog"].get("win16 API (W4: src/platform maps it to Win32)", []))
        parts.append("Win16 functions not provided: %d of 167" % left)
        done &= left == 0
    if tr:
        parts.append("framework trace: %d of the first %d calls match the oracle" % (tr["matching"], tr["n"]))
        if tr.get("matching_skipping_oracle_gaps", tr["matching"]) != tr["matching"]:
            parts.append("%d with tests/trace.py's ORACLE_GAPS skipped (a diagnostic, not the gate)"
                         % tr["matching_skipping_oracle_gaps"])
        done &= tr["matching"] >= tr["n"]
    else:
        parts.append("framework trace: not measured")
        done = False
    return ("DONE" if done else "open"), "; ".join(parts)


ROWS = [
    ("W0", "bootstrap", w0, "import/import.py --check, tools/ci_record.py"),
    ("W1", "compile", w1, "tools/compile_all.py --record [--toolchain winegcc]"),
    ("W2", "link", w2, "tools/undefined.py --record"),
    ("W3", "oracle", w3, "tests/oracle.py --selftest, tests/state.py --record, tests/diff.py --record"),
    ("W4", "platform", w4, "tools/undefined.py --record, tests/trace.py --gate --record"),
    ("W5", "asm", None, "differential on segments 17-19 (not written yet)"),
    ("W6", "lockstep", None, "tests/lockstep --corpus golden (not written yet)"),
    ("W7", "playable", None, "scripted sessions, saves both ways, human pass"),
    ("W8", "linux", None, "winegcc build through W3-W7"),
    ("W9", "64-bit", None, "W6 and W7 on x64, x86_64, macOS"),
    ("W10", "release", None, "a clean machine per OS reaches the map"),
]


def render():
    lines = ["# STATUS", "",
             "Generated by `tools/status.py` from `status/*.json`. Do not edit by hand: run the gate,",
             "then `python3 tools/status.py`.", "",
             "| | Milestone | State | Measured | Gate |", "| --- | --- | --- | --- | --- |"]
    current = None
    for key, name, fn, gate in ROWS:
        state, what = fn() if fn else ("--", "not measured")
        if current is None and state != "DONE":
            current = key
        lines.append("| %s | %s | %s | %s | `%s` |" % (key, name, state, what, gate))
    lines += ["", "Current milestone: **%s** (roadmap rule 1: one at a time, in order)." % (current or "none"), ""]
    return "\n".join(lines)


def main(argv):
    text = render()
    if "--check" in argv:
        have = open(STATUS).read() if os.path.exists(STATUS) else ""
        if have != text:
            print("STATUS.md is not what status/*.json says: run python3 tools/status.py")
            return 1
        print("STATUS.md is current")
        return 0
    open(STATUS, "w").write(text)
    print(text)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
