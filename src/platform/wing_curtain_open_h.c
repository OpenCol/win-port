/* wing_curtain_open_h: candidate for 1070:c5a4 (305 bytes).  Verdict: RELOCATABLE.
 * symbol: wing_curtain_open_h
 *
 * A transition: the old image (WinG surface `old_graf`) parts horizontally like a curtain -- its left half
 * slides left and its right half right, `step` columns per pass -- while the new image
 * (WinG surface `new_graf`) shows through the widening middle band; the port's palette is realised first
 * when it has one and DGROUP:0x3380 is 1. rect_width/rect_height are defined here because the
 * shipped calls to them are the compiler's own `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

extern int rect_width(RECT *r);

extern int rect_height(RECT *r);

void wing_curtain_open_h(GrafRec *old_graf, GrafRec *new_graf, MSWin *win, RECT *r, int step)
{
    int cols;
    int x;
    int w;
    int h;
    int y;
    int half_w;
    int k;

    w = rect_width(r);
    h = rect_height(r);
    x = r->left;
    y = r->top;
    half_w = rect_width(r) >> 1;
    if (old_graf != 0 && new_graf != 0 && win != 0) {
        if (win->hpal != 0 && g_palette_mode == 1)
            RealizePalette(win->hdc);
        for (cols = half_w; cols > 0; cols -= step) {
            k = cols * 2;
            WinGBitBlt(win->hdc, x, y, cols, h, old_graf->hdc, x + half_w - cols, y);
            WinGBitBlt(win->hdc, x + cols, y, w - k, h, new_graf->hdc, x + cols, y);
            WinGBitBlt(win->hdc, x + cols + (w - k), y, cols, h, old_graf->hdc, x + half_w, y);
        }
    }
}
