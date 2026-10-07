# tests/oracle

Milestone W3. Glue that uses col-win-re's `tools/re_agent/win16emu.py`, `winsim.py` and
`tools/playtest_diff.py` as a library to run the shipped `COLONIZE.EXE`: scripted input, the
cheat menu and `SETAUTO` for autoplay, a fixed tick so `randmize` seeds the same, and a dump of
every `model.json` global at each turn boundary. Gate: `tests/oracle --selftest` (two runs of
the shipped binary give identical dumps).
