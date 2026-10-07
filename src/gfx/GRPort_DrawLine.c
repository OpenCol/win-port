/* GRPort_DrawLine: candidate for 1068:5bc6 (311 bytes, 40 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_DrawLine
 *
 * Draws a line through the port's device context after rejecting lines outside
 * the clip rectangle at +8 (the right-hand test is `right < x2 && left < x1`, so a line that
 * ends past the right edge is rejected whenever it starts right of the left edge) and clamping both endpoints to it. The endpoints arrive as
 * four shorts and are clamped in place; the compiler keeps the first two in SI and DI.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

#include "records.h"
extern void MSDrawLine(GrafRec *win, int x1, int y1, int x2, int y2);

void GRPort_DrawLine(GRPort *p, short x1, short y1, short x2, short y2)
{
    if (p->rect.left > x1 && p->rect.left > x2)
        return;
    if (p->rect.right < x2 && p->rect.left < x1)
        return;
    if (p->rect.top > y1 && p->rect.top > y2)
        return;
    if (p->rect.bottom < y1 && p->rect.bottom < y2)
        return;
    if (p->rect.left > x1) x1 = p->rect.left;
    if (p->rect.right < x1) x1 = p->rect.right;
    if (p->rect.left > x2) x2 = p->rect.left;
    if (p->rect.right < x2) x2 = p->rect.right;
    if (p->rect.top > y1) y1 = p->rect.top;
    if (p->rect.bottom < y1) y1 = p->rect.bottom;
    if (p->rect.top > y2) y2 = p->rect.top;
    if (p->rect.bottom < y2) y2 = p->rect.bottom;
    MSDrawLine((GrafRec *)p->win, x1, y1, x2, y2);
}
