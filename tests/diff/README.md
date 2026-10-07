# tests/diff

Milestone W3. The per-function differential: col-win-re's `tools/re_agent/differential.py` runs
the original function under its Win16 harness, and the native function runs in-process over a
globals image loaded from the same staged state (`tests/state.py`, generated from `model.json`).
Return value and every global written are compared.
