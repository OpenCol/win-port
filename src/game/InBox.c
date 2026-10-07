/* InBox: candidate for 1048:3cda (64 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Whether a point lies strictly inside a rectangle given as an origin plus a width and a height.
 *
 * RENAMED 2026-09-26 from point_in_rect: the Mac build's InBox(short, short, short, short, short, short), bound by call-sequence alignment: in EuropeWin_EuropeMouseUnits the calls either side align and this is the one unbound callee left in that gap (0.5 votes, next 0.0); the arity agrees (6), and the body does what the Mac name says.
 */

#include "colonization.h"

int InBox(int x, int y, int rx, int ry, int rw, int rh)
{
    if (x > rx && y > ry && rx + rw > x && ry + rh > y)
        return 1;
    else
        return 0;
}
