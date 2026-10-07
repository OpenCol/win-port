/* EuropeWin_UpdateEurope: candidate for 1048:9d70 (129 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_UpdateEurope
 * name-source: sibling-build
 *
 * This module's copy of the cell blit: the cell is copied from the Europe window's offscreen
 * GRPort (+0x884) to the same rectangle offset by the window's origin at +0x76a. The twin of
 * ColonyWin_UpdateColony, differing only in where the port sits (+0x884, not +0x7a8).
 *
 * RENAMED 2026-09-26 from colony_blit_cell_1048_9d70: its object is the Europe window (the port at +0x884 is the GRPort EuropeWin_ctor builds there) and all seven callers are europe_* drawers; the colony window keeps ColonyWin_UpdateColony.
 *
 * RENAMED 2026-09-26 from europe_blit_cell: the Mac build's EuropeWin::UpdateEurope(short, short, short, short), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in EuropeWin_DrawUnits -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (1.0 votes, the next candidate 0.0); the arity agrees (5).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

void EuropeWin_UpdateEurope(EuropeWin *o, int x, int y, int w, int h)
{
    RECT r1;
    RECT r2;

    SetRect(&r1, x, y, x + w, y + h);
    CopyRect(&r2, &r1);
    OffsetRect(&r2, o->rect.left, o->rect.top);
    GRPort_CopyToPort(&o->port, (GRPort *)o, &r1, &r2);
}
