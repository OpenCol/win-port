/* draw_bevel -- shipped at 1068:c41f.
 *
 * Ported by tools/port.py from matched/gfx/draw_bevel.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal InflateRect(RECT *r, col_i16 dx, col_i16 dy);
#endif

extern void TextColor(col_i16 c);
extern void GRPort_DrawLine(void *p, col_i16 x1, col_i16 y1, col_i16 x2, col_i16 y2);


void draw_bevel(void *p, RECT *r, col_i16 depth, col_i16 c1, col_i16 c2)
{
    col_i16 lo;
    register col_i16 n;
    register col_i16 hi;

    n = depth;
    if (n > 0) {
        hi = c2;
        lo = c1;
    } else {
        hi = c1;
        lo = c2;
        n = -n;
    }
    while (n-- != 0) {
        TextColor(hi);
        GRPort_DrawLine(p, r->right - 1, r->top, r->right - 1, r->bottom - 1);
        GRPort_DrawLine(p, r->right - 1, r->bottom - 1, r->left, r->bottom - 1);
        TextColor(lo);
        GRPort_DrawLine(p, r->left, r->bottom - 1, r->left, r->top);
        GRPort_DrawLine(p, r->left, r->top, r->right - 1, r->top);
        InflateRect(r, -1, -1);
    }
}
