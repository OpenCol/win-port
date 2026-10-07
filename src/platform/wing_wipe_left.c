/* wing_wipe_left: candidate for 1070:c1b1 (191 bytes).  Verdict: RELOCATABLE.
 * symbol: wing_wipe_left
 *
 * A transition: the rectangle's image is revealed from the right edge leftwards, `step` columns
 * at a time, each pass blitting the left `cols` columns of the source WinG surface (`src`, a GrafRec) to the right end
 * of the rectangle in the port (palette realised first when the port has one and DGROUP:0x3380 is 1); nothing
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

void wing_wipe_left(GrafRec *old_graf, GrafRec *src, MSWin *win, RECT *r, int step)
{
    int w;
    int h;
    int x;
    int y;
    int i;
    int cols;

    cols = 0;
    w = rect_width(r);
    h = rect_height(r);
    x = r->left;
    y = r->top;
    if (old_graf != 0 && src != 0 && win != 0) {
        if (win->hpal != 0 && g_palette_mode == 1)
            RealizePalette(win->hdc);
        for (i = 0, cols = 0; i < w; i += step, cols += step)
            WinGBitBlt(win->hdc, x + w - cols, y, cols, h, src->hdc, x, y);
    }
}
