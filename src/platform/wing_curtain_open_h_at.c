/* wing_curtain_open_h_at: candidate for 1070:c6d5 (342 bytes).  Verdict: RELOCATABLE.
 * symbol: wing_curtain_open_h_at
 *
 * A transition: the old image (WinG surface `old_graf`) parts horizontally like a curtain -- its left half
 * slides left and its right half right, `step` columns per pass -- while the new image
 * (WinG surface `new_graf`) shows through the widening middle band, the result placed at an offset (dx, dy)
 * in the port (the old image is read at that offset too); the port's palette is realised first
 * when it has one and DGROUP:0x3380 is 1. rect_width/rect_height are defined here because the
 * shipped calls to them are the compiler's own `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

extern int rect_width(RECT *r);

extern int rect_height(RECT *r);

void wing_curtain_open_h_at(GrafRec *old_graf, GrafRec *new_graf, MSWin *win, RECT *r, int dx, int dy, int step)
{
    int cols;
    int x;
    int w;
    int h;
    int y;
    int half;
    int k;

    w = rect_width(r);
    h = rect_height(r);
    x = r->left;
    y = r->top;
    half = rect_width(r) >> 1;
    if (old_graf != 0 && new_graf != 0 && win != 0) {
        if (win->hpal != 0 && g_palette_mode == 1)
            RealizePalette(win->hdc);
        for (cols = half; cols > 0; cols -= step) {
            k = cols * 2;
            WinGBitBlt(win->hdc, x + dx, y + dy, cols, h, old_graf->hdc, x + half - cols + dx, y + dy);
            WinGBitBlt(win->hdc, x + cols + dx, y + dy, w - k, h, new_graf->hdc, x + cols, y);
            WinGBitBlt(win->hdc, x + cols + dx + (w - k), y + dy, cols, h, old_graf->hdc, x + half + dx, y + dy);
        }
    }
}
