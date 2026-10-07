/* EuropeMouseArea: candidate for 1048:4235 (147 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 * symbol: EuropeMouseArea
 *
 * The dispatcher the six hit_region_* tests belong to.  It walks them in a
 * fixed order and returns the id of the first rectangle that contains the
 * point, or 15 when the point is outside every one of them.  Region 1 has an
 * inner rectangle: a hit there reports 10 instead of 1.
 *
 * The six tests are calls to code earlier in the same module, so they
 * are compiled in this file rather than declared extern.
 *
 * RENAMED 2026-09-26 from region_at: the Mac build's EuropeMouseArea(short, short), bound by call-sequence alignment: in EuropeWin_MouseMove the calls either side align and this is the one unbound callee left in that gap (0.5 votes, next 0.0); the arity agrees (2), and the body does what the Mac name says.
 *
 * RENAMED 2026-09-27 from EuropeWin_EuropeMouseArea: the Mac build's EuropeMouseArea, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Its own header records it as the Mac EuropeMouseArea(short, short), the region dispatcher over the six box tests (inMarketBox, ...), but the file spells it with a EuropeWin_ class prefix, and the Mac routine is a free function (__Fss) with no this; 2 args; ratio 1.06. This only corrects the spelling.
 */

#include "colonization.h"

extern int inMarketBox(int x, int y);

extern int inInboundBox(int x, int y);

extern int inOutboundBox(int x, int y);

extern int inShipyard(int x, int y);

extern int inUnitsBox(int x, int y);

extern int inCargoBox(int x, int y);

int EuropeMouseArea(int x, int y)
{
    int r = 15;
    if (inMarketBox(x, y))
        r = 0;
    else if (inInboundBox(x, y))
        r = 2;
    else if (inOutboundBox(x, y))
        r = 3;
    else if (inShipyard(x, y)) {
        r = 1;
        if (inCargoBox(x, y))
            r = 10;
    } else if (inUnitsBox(x, y))
        r = 4;
    return r;
}
