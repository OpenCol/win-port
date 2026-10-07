/* SetNations_TrackMouse: candidate for 1030:05c7 (116 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SetNations_TrackMouse
 * name-source: sibling-build
 *
 * Which of four fixed panels a point falls in, numbered 1 to 4 by column then row, or 0 outside them all.
 *
 * RENAMED 2026-09-26 from panel_at: the Mac build's SetNations::TrackMouse(short, short), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between SetNations_Init and SetDifficulty_DrawIt, where the Mac gap holds 4 functions and the Windows one 5, so the pairing inside it is by size and arity (the Mac body is 1.17x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order. As SetDifficulty's: the Windows hit test is SetNations::TrackMouse by layout.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
int SetNations_TrackMouse(SetNations *unused, int x, int y)
{
    int r = 0;
    if (x >= 0xdf && x <= 0x18f) {
        if (y >= 0x1e && y <= 0xe6)
            r = 1;
        else if (y >= 0xfa && y <= 0x1c2)
            r = 3;
    } else if (x >= 0x1a6 && x <= 0x256) {
        if (y >= 0x1e && y <= 0xe6)
            r = 2;
        else if (y >= 0xfa && y <= 0x1c2)
            r = 4;
    }
    return r;
}
