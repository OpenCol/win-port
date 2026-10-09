#!/bin/sh
# Publish a release of the port (roadmap W10):  tools/release.sh v0.1.0 [--final]
#
# Checks that main is clean, pushed, and green on CI at this commit, then runs the ci workflow by
# hand with the version (gh workflow run): CI builds the four kits from this commit, writes the
# notes (tools/release_notes.py) and creates the GitHub release with its tag. Without --final the
# release is marked a pre-release. Then it follows the run and prints the release's address.
set -e
v="$1"
pre=true
[ "$2" = "--final" ] && pre=false
case "$v" in
    v[0-9]*) ;;
    *) echo "usage: tools/release.sh vX.Y.Z [--final]" >&2; exit 2 ;;
esac
cd "$(dirname "$0")/.."
[ "$(git rev-parse --abbrev-ref HEAD)" = main ] || { echo "not on main" >&2; exit 1; }
[ -z "$(git status --porcelain)" ] || { echo "the tree has changes: commit them first" >&2; exit 1; }
git fetch -q origin main
head=$(git rev-parse HEAD)
[ "$head" = "$(git rev-parse origin/main)" ] || { echo "main is not pushed (or is behind origin)" >&2; exit 1; }
if git ls-remote --exit-code --tags origin "refs/tags/$v" >/dev/null 2>&1; then
    echo "$v exists already" >&2; exit 1
fi
ci=$(gh run list --branch main --commit "$head" --workflow ci.yml --json conclusion,status \
     --jq '[.[] | select(.status == "completed")][0].conclusion // "none"')
[ "$ci" = success ] || { echo "CI at ${head%"${head#???????}"} is '$ci', not success: wait for it or fix it" >&2; exit 1; }

echo "releasing $v from $head (pre-release: $pre)"
gh workflow run ci.yml --ref main -f version="$v" -f prerelease="$pre"
sleep 10
run=$(gh run list --workflow ci.yml --event workflow_dispatch --limit 1 --json databaseId --jq '.[0].databaseId')
echo "CI run $run"
gh run watch "$run" --exit-status >/dev/null
gh release view "$v" --json url --jq .url
