#!/usr/bin/env python3
"""Record the latest finished CI run on GitHub in status/ci.json (for tools/status.py).

    python3 tools/ci_record.py

Needs the GitHub CLI (`gh`), signed in. Records the run for the commit currently checked
out when one has finished, else the newest finished run on main, with its conclusion.
"""
import json
import os
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def main():
    head = subprocess.run(["git", "-C", ROOT, "rev-parse", "HEAD"], capture_output=True, text=True).stdout.strip()
    r = subprocess.run(["gh", "run", "list", "--branch", "main", "--limit", "20", "--json",
                        "headSha,conclusion,status,databaseId,workflowName"],
                       cwd=ROOT, capture_output=True, text=True)
    if r.returncode:
        sys.exit("gh run list failed: %s" % r.stderr.strip())
    runs = [x for x in json.loads(r.stdout) if x["status"] == "completed"]
    if not runs:
        sys.exit("no finished CI run yet")
    pick = next((x for x in runs if x["headSha"] == head), runs[0])
    out = {"commit": pick["headSha"], "conclusion": pick["conclusion"], "run": pick["databaseId"],
           "workflow": pick["workflowName"]}
    os.makedirs(os.path.join(ROOT, "status"), exist_ok=True)
    with open(os.path.join(ROOT, "status", "ci.json"), "w") as f:
        json.dump(out, f, indent=1, sort_keys=True)
        f.write("\n")
    print("CI %s at %s (run %s)" % (out["conclusion"], out["commit"][:7], out["run"]))
    return 0 if out["conclusion"] == "success" else 1


if __name__ == "__main__":
    sys.exit(main())
