/* wing_curtain_open_v: candidate for 1070:c32f (302 bytes).  Verdict: RELOCATABLE.
 * symbol: wing_curtain_open_v
 *
 * A transition: the old image (window `a`) parts vertically like a curtain -- its upper half
 * slides up and its lower half down, `step` rows per pass -- while the new image (window `b`)
 * shows through the widening middle band; the port's palette is realised first when it has
 * one and DGROUP:0x3380 is 1. rect_width/rect_height are defined here because the shipped
 * calls to them are the compiler's own `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

extern int rect_width(RECT *r);

extern int rect_height(RECT *r);

void wing_curtain_open_v(GrafRec *old_graf, GrafRec *new_graf, MSWin *win, RECT *r, int step)
{
    int rows;
    int x;
    int w;
    int h;
    int y;
    int h2;
    int k;

    w = rect_width(r);
    h = rect_height(r);
    x = r->left;
    y = r->top;
    rows = h2 = rect_height(r) >> 1;
    if (old_graf != 0 && new_graf != 0 && win != 0) {
        if (win->hpal != 0 && g_palette_mode == 1)
            RealizePalette(win->hdc);
        for (rows = h2; rows > 0; rows -= step) {
            k = rows * 2;
            WinGBitBlt(win->hdc, x, y, w, rows, old_graf->hdc, x, y + h2 - rows);
            WinGBitBlt(win->hdc, x, y + rows, w, h - k, new_graf->hdc, x, y + rows);
            WinGBitBlt(win->hdc, x, y + rows + (h - k), w, rows, old_graf->hdc, x, y + h2);
        }
    }
}
