# Where this stands

Last green commit: **f239ea0**, 2,388 ported. 476 verified by both harnesses, 59 on the
host only, 1,683 in 16 bits only, 170 by neither with a reason from both, 0 uncovered,
0 disagreements, 1,070 mutations caught. Links (NE, 43 segments, 807 translation units,
346 stubs) and boots.

## Uncommitted, and NOT verified

The working tree is at **2,406 cases** with two porter changes on top of that commit:

  col/win16.h forward-declares Dlg, GameWin, MrTimer and Rec4c28 as `typedef struct X X;`
  and a port defining its own anonymous `typedef struct { ... } Dlg;` collided with it.
  The tag is COMPLETED now -- `struct Dlg { ... };` -- which is the same object and
  satisfies both. 13 files.

  A file-local type whose name collides with one col/win16.h defines is renamed, the way
  a collision with a record's name already was. RGBQUAD with `reserved` where the header
  says `rgbReserved` is not the same struct.

The source corpus has also grown from 3,173 to 3,188 reconstructions and those are
ported here.

A full run on this reached 2,408 ported with TWO disagreements -- map_planes_free and
text_message_show, both functions that only became portable once the type collisions
were gone. Both are withdrawn in model/withdrawn.json with what the harness observed,
and the confirming run after that withdrawal was stopped partway. So: the withdrawals
are in, the re-run is not done, and nothing here is claimed.

Resume with `bash tools/check.sh > /tmp/check.txt 2>&1`; about an hour. Commit only on
ALL GREEN.

## The worklist

`tools/port.py --roots` follows every "calls X, which is not ported" chain to the
refusals the rest rest on. That is what to work from.

`tools/link.py` reports 51 callees DECLARED TWO WAYS across the corpus. A one-function-
per-file build never compares two files' prototypes, so grouping them for the link is
the first time this corpus has been asked to agree with itself. Each one is either a
naming error or a real type disagreement, and neither has been looked at.

The twelve withdrawn in model/withdrawn.json with "the cause is not yet found" are the
other thread. `europe_transfer_cargo` writes `8C D0 90 45 55 8B` into the staged region
-- instruction bytes, `MOV AX,SS` -- which is what a near pointer relative to SS looks
like read as a far one. `MSInitButton` fails to compile on the same thing, where Borland
spells it `Region _ss *`. An _ss pointer read as far is the thing to chase.

## What the oracles caught, and what it cost to believe them

Seven of the nine defects this session were in the TOOLS, not the ports, and every one
of them made a wrong answer look like a right one:

  g_game_flags spelt a byte where the shipped code writes a word, so `^= 0x2000`
  truncated to zero and the port flipped nothing. model/widths.json now records a width
  a WRITER settles, with the instruction that settles it.

  An opaque `class GRPort { char body[0x26]; GRPort(); ~GRPort(); }` became a plain
  struct, and `GRPort port;` stopped calling the constructor and destructor. Seven
  handlers agreed on every write and every returned value while calling no imports at
  all, because the destructor is what reaches InvalidateRect.

  `Rec` is a placeholder name the model merged across unrelated structs. A file walking
  202-byte records strode by 1,916. A name match is not evidence; the porter now
  requires the stride to agree wherever the file subscripts the type.

  One shard died and took 81 cases with it; its partial results merged as complete and
  coverage called the survivors uncovered.

  A `pascal` PUBDEF is UPPERCASE with no underscore. Six window procedures read as
  disagreements; they agree 81/81.

  boot.py reported OK on a day-old executable after the link had failed.

  glob on macOS asks a case-insensitive filesystem, so `src/*/ATOL.c` matched
  `src/game/atol.c` -- a different function -- and ATOL was taken for ported.

And one I inflicted: stubbing every undefined symbol in the host archive stubbed printf,
memcpy and malloc too, which took 469 dual-verified functions to 0 and left the mutation
selftest nothing to mutate. Ask the LINKER what is missing, never `nm`.

## Still open

`win-port` has no git remote, so nothing has been pushed. A URL is all it needs.
