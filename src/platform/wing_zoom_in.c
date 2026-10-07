/* wing_zoom_in: candidate for 1070:c82b (286 bytes).  Verdict: RELOCATABLE.
 * symbol: wing_zoom_in
 *
 * A transition: the new image (the GrafRec `source`, drawn into the MSWin `win`) grows from the rectangle's centre, each pass
 * stretching the whole source into a box (`dw` x `dh`) two pixels wider and taller than the
 * last, until the shorter half-extent is used up, then the final full-size copy; the port's palette is
 * realised first when it has one and DGROUP:0x3380 is 1. rect_width, rect_height and the
 * module's min_int are defined here because the shipped calls to them are the compiler's own
 * `push cs / call `.
 *
 * The first argument is the old port's GrafRec, as for wing_scroll_up and as every caller
 * passes (2026-10-02; it was `MSWin *`).
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

extern int rect_width(RECT *r);

extern int rect_height(RECT *r);

void wing_zoom_in(GrafRec *old_graf, GrafRec *source, MSWin *win, RECT *r, int step)
{
    int i;
    int dw;
    int w;
    int h;
    int x;
    int y;
    int dh;
    int hw;
    int hh;
    int n;

    w = rect_width(r);
    h = rect_height(r);
    x = r->left;
    y = r->top;
    dw = dh = 1;
    hw = w >> 1;
    hh = h >> 1;
    if (old_graf != 0 && source != 0 && win != 0) {
        if (win->hpal != 0 && g_palette_mode == 1)
            RealizePalette(win->hdc);
        n = min_int_1070_ddd8(hw, hh);
        for (i = n; i > 0; dw += 2, dh += 2) {
            WinGStretchBlt(win->hdc, hw-- + x, hh-- + y, dw, dh, source->hdc, x, y, w, h);
            i--;
        }
        WinGBitBlt(win->hdc, x, y, w, h, source->hdc, x, y);
    }
}
