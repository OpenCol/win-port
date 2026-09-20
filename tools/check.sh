#!/usr/bin/env bash
# Everything this repo can check about itself, in the order a failure is cheapest to read.
#
#   tools/check.sh          the lot
#   tools/check.sh fast     skip the Borland compile (needs DOSBox-X and the toolchain)
set -uo pipefail
cd "$(dirname "$0")/.."
SRC=${COL_SRC_REPO:-$HOME/Repositories/dderevjanik/col-win-re}
UC="$SRC/vendor/re-agent-venv/bin/python"
rc=0
run() { echo "== $*"; "$@" || rc=1; }

run python3 tools/extract_segments.py --check
run python3 tools/gen_model_header.py --check
# Is the extracted data really the shipped image? An independent loader says so.
[ -x "$UC" ] && run "$UC" tests/verify_segments.py "$SRC"
run python3 tests/differential.py --all --selftest
run python3 tests/differential.py --all
if [ "${1:-}" != "fast" ]; then
  # The link is the only check that covers whether the pieces FIT: one definition each,
  # the segments inside 64 KiB, DGROUP not overflowing, imports resolved, resources bound.
  # The same question in 16 bits: the same sources, compiled by Borland and run in the
  # emulator beside the shipped function, where far pointers, segment wrap and stack
  # discipline are native rather than approximated.
  [ -x "$UC" ] && run "$UC" tests/emu_differential.py --all
  run python3 tools/link.py
  [ -x "$UC" ] && run "$UC" tests/boot.py
fi
[ $rc -eq 0 ] && echo "ALL GREEN" || echo "FAILURES ABOVE"
exit $rc
