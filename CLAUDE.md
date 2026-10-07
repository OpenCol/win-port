# CLAUDE.md

A native Win32 port of *Colonization for Windows*, built from col-win-re's byte-matched
reconstruction. The goal and the rules are col-win-re's roadmap. Before writing any code, read
these three documents in full, at the commit this repository imports:

- [win-port-roadmap.md](https://github.com/dderevjanik/col-win-re/blob/508d4d0d0c12b856a7f33534ae79c5af48c14939/docs/reconstruction/win-port-roadmap.md)
  (locally: `$COL_WIN_RE/docs/reconstruction/win-port-roadmap.md`): decisions, milestones,
  gates.
- [port-brief.md](https://github.com/dderevjanik/col-win-re/blob/508d4d0d0c12b856a7f33534ae79c5af48c14939/docs/reconstruction/port-brief.md):
  what the corpus is, and which of its constraints do *not* carry over.
- [PROMPT.md](https://github.com/dderevjanik/col-win-re/blob/508d4d0d0c12b856a7f33534ae79c5af48c14939/PROMPT.md)
  §0.5: "linked" is not "ran", and "ran" is not "works".

The roadmap's section 1 decisions are fixed: Win32; 32-bit x86 first; MinGW or clang, and MSVC
x86 must also build; Winelib for Linux and macOS; game integers as `i16`/`u16`; the real class
hierarchy; assets read from the NE DLLs; WinG replaced by a DIB section; MCI with a file
fallback; game files never committed. Changing one is a question for a person (rule 7).

## Rules (roadmap section 4)

1. **One milestone at a time, in order.** W3 must be green before any work on W4–W7 lands.
2. **Each session:** read `STATUS.md`, then pick the cheapest open item of the current milestone.
   Work in small commits. Run that milestone's fast gate before each commit, regenerate
   `STATUS.md` (`python3 tools/status.py`), then commit and push.
3. **Claims need command output.** A milestone is "done" only when its gate's exit-0 output is
   recorded in `STATUS.md`. "Compiles", "links", "runs" and "matches" are four different claims.
4. **Never change game behaviour to make a test pass.** When lockstep diverges, find the function
   that *writes* the diverging value and fix the port's reading of the original. If the corpus
   itself is wrong, record it in `UPSTREAM.md` with evidence. Never edit col-win-re.
5. **`PORT:` comments on every body edit.** `tools/port_diff.py` must list them all. An unmarked
   behavioural edit is a defect, and `import/import.py --check` fails on it.
6. **Time box.** If one function or one divergence survives about 25 attempts, move it to
   `BLOCKED.md` with what was tried and the smallest reproducer, then go on.
7. **Ask a person** for anything needing visual or aesthetic judgement, licensing, or a change to
   a section 1 decision. Do not ask about anything a gate can answer.
8. **Never commit game files**, oracle recordings or decompiled assets. Only hashes go in git.

## This repository's mechanics

- `import/SOURCE` pins the col-win-re commit. `python3 import/import.py --check` rebuilds the
  import in a temporary directory, from `git archive` of that commit; it never reads
  col-win-re's working tree. It records `status/import.json`. Set `COL_WIN_RE` if col-win-re is
  not at `../../dderevjanik/col-win-re`.
- Moving `SOURCE` forward is a three-way merge once any file carries `PORT:` edits; the plain
  import refuses to overwrite them.
- The import owns `src/game`, `src/gfx`, `src/platform` and the top-level `include/*.h`. This
  repository's own headers go in `include/port/`, its own code in `src/host` and `src/asm`.
- `STATUS.md` is generated. A gate records its result in `status/<gate>.json`, and
  `tools/status.py` renders the page; CI fails when the page and the records disagree.
  `tools/ci_record.py` records the latest CI run.
- Branch `archive/win16` holds the first win-port (Win16, Borland, behaviour-exact), kept for
  reference. Its decisions are superseded by the roadmap's section 1.
