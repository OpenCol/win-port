/* inInboundBox: candidate for 1048:4123 (55 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * One of six point-in-rectangle tests read by the region dispatcher at
 * 1048:4235, which returns this rectangle's id (2) when the point falls
 * inside it.  The rectangle is given in screen coordinates and its edges are
 * exclusive on both sides, exactly as the shipped comparisons read.
 *
 * RENAMED 2026-09-27 from hit_region_2: the Mac build's inInboundBox(short, short), one of EuropeMouseArea's six box tests: bound by tools/mac_bind.py (call-sequence alignment and source layout agree) and by the region id its callers test -- 0 EuropeWin_EuropeMouseMarket, 10 EuropeWin_EuropeMouseShipsCargo, 1 EuropeWin_EuropeMouseShipsProper, 4 the dock units, 2/3 the two voyage lists.
 */

#include "colonization.h"

int inInboundBox(int x, int y)
{
    int inside = 0;
    if (x < 0x11a && x > 0x8f && y < 0x17b && y > 0x102)
        inside = 1;
    return inside;
}
