/* wing_wipe_down: candidate for 1070:c0f2 (191 bytes).  Verdict: RELOCATABLE.
 * symbol: wing_wipe_down
 *
 * A transition: the rectangle's image slides in from above, `step` rows at a time, each pass
 * blitting the bottom `rows` rows of the source surface (the GrafRec) to the top of the rectangle in the window (palette realised first when the window has one and DGROUP:0x3380 is 1); nothing
 * when any of the three objects is null. rect_width/rect_height are defined here because the
 * shipped calls to them are the compiler's own `push cs / call `.
 *
 * The first argument is the old port's GrafRec, as for wing_scroll_up and as every caller
 * passes (2026-10-02; it was `MSWin *`).
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

extern int rect_width(RECT *r);

extern int rect_height(RECT *r);

void wing_wipe_down(GrafRec *old_graf, GrafRec *src, MSWin *win, RECT *r, int step)
{
    int w;
    int h;
    int x;
    int y;
    int i;
    int rows;

    rows = 0;
    w = rect_width(r);
    h = rect_height(r);
    x = r->left;
    y = r->top;
    if (old_graf != 0 && src != 0 && win != 0) {
        if (win->hpal != 0 && g_palette_mode == 1)
            RealizePalette(win->hdc);
        for (i = 0, rows = 0; i < h; i += step, rows += step)
            WinGBitBlt(win->hdc, x, y, w, rows, src->hdc, x, y + h - rows);
    }
}
