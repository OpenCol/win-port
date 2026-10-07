/* inCargoBox: candidate for 1048:41fe (55 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * One of six point-in-rectangle tests read by the region dispatcher at
 * 1048:4235, which returns this rectangle's id (10) when the point falls
 * inside it.  The rectangle is given in screen coordinates and its edges are
 * exclusive on both sides, exactly as the shipped comparisons read.
 *
 * RENAMED 2026-09-27 from hit_region_10: the Mac build's inCargoBox(short, short), one of EuropeMouseArea's six box tests: bound by tools/mac_bind.py (call-sequence alignment and source layout agree) and by the region id its callers test -- 0 EuropeWin_EuropeMouseMarket, 10 EuropeWin_EuropeMouseShipsCargo, 1 EuropeWin_EuropeMouseShipsProper, 4 the dock units, 2/3 the two voyage lists.
 */

#include "colonization.h"

int inCargoBox(int x, int y)
{
    int inside = 0;
    if (x < 0x1b0 && x > 0x122 && y < 0x190 && y > 0x16e)
        inside = 1;
    return inside;
}
