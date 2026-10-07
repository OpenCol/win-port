/* ColonistIcon: candidate for 1040:1d76 (109 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The portrait id for a speciality 0x13..0x1c (jump table read from the
 * image): 0x14..0x16 give 0x3b..0x3d, 0x18..0x1b give 0x3e, 0x6b, 0x6c,
 * 0x43, 0x13 and 0x1c give 0x65; anything else is the speciality + 0x52.

 * SIZE: the census says 89 and the function is 109. The extra bytes are the CS-relative
 * jump table for this function's `switch`, which Borland puts in the CODE segment and
 * Ghidra's reachable-code size excludes. The reconstruction grades over the whole
 * extent unchanged; only the number moved.
 *
 * RENAMED 2026-09-26 from specialist_portrait: the Mac build's ColonistIcon(short), bound by call-sequence alignment (4.5 aligned gaps in ReportWin_ReportsLabor, ReportWin_Score, combat_fight leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.05x this one.
 */

#include "colonization.h"

int ColonistIcon(int spec)
{
    int r;

    switch (spec) {
    case 0x14:
        r = 0x3b;
        break;
    case 0x15:
        r = 0x3c;
        break;
    case 0x16:
        r = 0x3d;
        break;
    case 0x18:
        r = 0x3e;
        break;
    case 0x19:
        r = 0x6b;
        break;
    case 0x1a:
        r = 0x6c;
        break;
    case 0x1b:
        r = 0x43;
        break;
    case 0x13:
    case 0x1c:
        r = 0x65;
        break;
    default:
        r = spec + 0x52;
        break;
    }
    return r;
}
