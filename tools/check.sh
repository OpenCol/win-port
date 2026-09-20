#!/usr/bin/env bash
# Everything this repo can check about itself, in the order a failure is cheapest to read.
#
#   tools/check.sh          the lot
#   tools/check.sh fast     skip the Borland compile (needs DOSBox-X and the toolchain)
set -uo pipefail
cd "$(dirname "$0")/.."
rc=0
run() { echo "== $*"; "$@" || rc=1; }

run python3 tools/gen_model_header.py --check
run python3 tests/differential.py --all --selftest
run python3 tests/differential.py --all
if [ "${1:-}" != "fast" ]; then
  run python3 tools/bcc.py src/world.c
  for f in src/game/*.c; do run python3 tools/bcc.py "$f"; done
fi
[ $rc -eq 0 ] && echo "ALL GREEN" || echo "FAILURES ABOVE"
exit $rc
