/* wing_blit_to_window: candidate for 1070:8e7a (172 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `wing_grab_from_window` RUN BACKWARDS: the same window DC, the same palette
 * dance when the window has one and DGROUP:0x3380 is 1, the same `WinGBitBlt` -- but the
 * screen DC is the DESTINATION here and the WinG surface the source. The grab reads the
 * screen into a surface; this one puts a surface on the screen.
 *
 * The direction is visible in one push: `WinGBitBlt` is `` and its first argument is
 * the destination, and the first thing pushed is `SI`, which holds `GetDC`'s result.
 *
 * Two rectangles: the source rectangle supplies the size and the source origin, the
 * destination rectangle only the origin. The size comes from the SOURCE, which is why
 * `rect_width` and `rect_height` are called on `[bp+0xe]` and never on `[bp+0x12]`.
 *
 * Both are this module's -called copies and are stubs here (rule 47); everything else is
 * an import.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

/* same object: 4-byte `push cs; call ` */

void wing_blit_to_window(GrafRec *src, MSWin *win, RECT *sr, RECT *dr)
{
    unsigned dc;

    if (win != 0 && src != 0) {
        dc = GetDC(win->hwnd);
        if (win->hpal != 0 && g_palette_mode == 1) {
            SelectPalette(dc, win->hpal, 0);
            RealizePalette(dc);
        }
        WinGBitBlt(dc, dr->left, dr->top, rect_width_1070_962a(sr), rect_height_1070_9649(sr),
                   src->hdc, sr->left, sr->top);
        ReleaseDC(win->hwnd, dc);
    }
}
