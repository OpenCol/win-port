/* GRPort_DrawRect: candidate for 1068:5cfd (417 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_DrawRect
 *
 * Draws a rectangle outline in the port: nothing when the rectangle lies wholly outside the
 * port's clip rectangle (+8..+0xe); otherwise a copy clamped to the clip rectangle is drawn
 * as four lines (top, right, bottom, left) with MSDrawLine on the port's window (+0x22).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

extern void MSDrawLine(GrafRec *win, int x1, int y1, int x2, int y2);

void GRPort_DrawRect(GRPort *p, RECT *r)
{
    RECT c;

    if (p->rect.left > r->left && r->right < p->rect.left)
        return;
    if (p->rect.right < r->left && r->right > p->rect.left)
        return;
    if (r->top < p->rect.top && r->bottom < p->rect.top)
        return;
    if (r->top > p->rect.bottom && r->bottom > p->rect.bottom)
        return;
    CopyRect(&c, r);
    if (p->rect.left > r->left)
        c.left = p->rect.left;
    if (r->right > p->rect.right)
        c.right = p->rect.right;
    if (r->top < p->rect.top)
        c.top = p->rect.top;
    if (r->bottom > p->rect.bottom)
        c.bottom = p->rect.bottom;
    MSDrawLine((GrafRec *)p->win, c.left, c.top, c.right, c.top);
    MSDrawLine((GrafRec *)p->win, c.right, c.top, c.right, c.bottom);
    MSDrawLine((GrafRec *)p->win, c.right, c.bottom, c.left, c.bottom);
    MSDrawLine((GrafRec *)p->win, c.left, c.bottom, c.left, c.top);
}
