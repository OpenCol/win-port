/* type_of_army: candidate for 1038:47ab (162 bytes, 3 call sites).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Maps a small mode plus an index to one of seven codes. Modes 2 and 3 are constants;
 * mode 1 and the default share a three-way test on the same record array that
 * 1018:394a validates against (stride 0x34 at 0x1098:0x7841) and on bit 0 of a byte
 * global, differing only in which codes they yield.
 *
 * `case 0:` falling straight into `default:` is what makes this a jump table. With
 * only cases 1, 2 and 3 declared, Borland emits a compare chain -- `CMP AX,1 / JZ ...`
 * -- and diverges at +0xe. Adding the empty `case 0:` label gives the table a fourth
 * entry and produces the shipped `CMP BX,3 / JA default / ADD BX,BX / JMP CS:[BX+..]`.
 * The label costs no bytes of its own; it changes the dispatch strategy.
 *
 * The case bodies appear in the object in the order 2, 3, 1, default, which is the
 * order they are written in below: Borland lays them out in source order, so the
 * emitted order is evidence about the source and not an artefact.
 *
 * The inner conditionals are conditional expressions, not if-statements -- every arm
 * produces its value in AX and there is a single `MOV CX,AX` after the join.
 *
 * Verdict RELOCATABLE.
 * Re-verified 2026-09-10 at the full 154-byte extent, trailing dead epilogue included: RELOCATABLE.

 * SIZE: the census says 154 and the function is 162. The extra bytes are the CS-relative
 * jump table for this function's `switch`, which Borland puts in the CODE segment and
 * Ghidra's reachable-code size excludes. The reconstruction grades over the whole
 * extent unchanged; only the number moved.
 *
 * RENAMED 2026-09-26 from select_state_code: the Mac build's type_of_army(short, short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between rebel_level and intern, which are already bound; the arity agrees and the Mac body is 0.99x this one.
 */

#include "colonization.h"

#include "control.h"
#include "gameflag.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

int type_of_army(int kind, int nation)
{
    int r;

    switch (kind) {
    case 2:
        r = 0x12;
        break;
    case 3:
        r = 0xb;
        break;
    case 1:
        r = (nation < 4 && g_nation_text[nation].control == CONTROL_HUMAN) ? ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) ? 7 : 4) : 8;
        break;
    case 0:
    default:
        r = (nation < 4 && g_nation_text[nation].control == CONTROL_HUMAN) ? ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) ? 9 : 4) : 6;
        break;
    }
    return r;
}
