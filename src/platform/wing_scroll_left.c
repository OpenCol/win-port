/* wing_scroll_left: candidate for 1070:bf43 (240 bytes).  Verdict: RELOCATABLE.
 * symbol: wing_scroll_left
 *
 * A transition: the old image (window `a`) scrolls left out of the rectangle while the new one
 * (window `b`) scrolls in from the right, `step` columns per pass, both blitted into the port (palette
 * realised first when the port has one and DGROUP:0x3380 is 1); nothing when any object is
 * null. rect_width/rect_height are defined here because the shipped calls to them are the
 * compiler's own `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

extern int rect_width(RECT *r);

extern int rect_height(RECT *r);

void wing_scroll_left(GrafRec *old_graf, GrafRec *new_graf, MSWin *win, RECT *r, int step)
{
    int cols;
    int x;
    int w;
    int h;
    int y;
    int i;

    cols = 0;
    w = rect_width(r);
    h = rect_height(r);
    x = r->left;
    y = r->top;
    if (old_graf != 0 && new_graf != 0 && win != 0) {
        if (win->hpal != 0 && g_palette_mode == 1)
            RealizePalette(win->hdc);
        for (i = 0, cols = 0; i < w; i += step, cols += step) {
            WinGBitBlt(win->hdc, x, y, w - cols, h, old_graf->hdc, x + cols, y);
            WinGBitBlt(win->hdc, x + w - cols, y, cols, h, new_graf->hdc, x, y);
        }
    }
}
