/* wing_scroll_up: candidate for 1070:bc77 (238 bytes).  Verdict: RELOCATABLE.
 * symbol: wing_scroll_up
 *
 * A transition: the old image (WinG surface `old_graf`) scrolls up out of the rectangle while the new one
 * (WinG surface `new_graf`) scrolls in from below, `step` rows per pass, both blitted into the port (palette
 * realised first when the port has one and DGROUP:0x3380 is 1); nothing when any object is
 * null. rect_width/rect_height are defined here because the shipped calls to them are the
 * compiler's own `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

extern int rect_width(RECT *r);

extern int rect_height(RECT *r);

void wing_scroll_up(GrafRec *old_graf, GrafRec *new_graf, MSWin *win, RECT *r, int step)
{
    int rows;
    int x;
    int w;
    int h;
    int y;
    int i;

    rows = 0;
    w = rect_width(r);
    h = rect_height(r);
    x = r->left;
    y = r->top;
    if (old_graf != 0 && new_graf != 0 && win != 0) {
        if (win->hpal != 0 && g_palette_mode == 1)
            RealizePalette(win->hdc);
        for (i = 0, rows = 0; i < h; i += step, rows += step) {
            WinGBitBlt(win->hdc, x, y, w, h - rows, old_graf->hdc, x, y + rows);
            WinGBitBlt(win->hdc, x, y + h - rows, w, rows, new_graf->hdc, x, y);
        }
    }
}
