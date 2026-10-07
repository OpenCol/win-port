/* PORT: this file's own declarations of GRPort_GetWin_1070_960d, get_win_1070_95f0 removed: the header declares them from the definition (W1 step 3). */
/* wing_child_paint: candidate for 1070:05a9 (336 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * The paint body for a WinG-backed child window. The PARENT window's long at index 12 decides
 * which of the two halves runs: set, and the damaged rectangle is blitted out of the parent's
 * off-screen bitmap with `WinGBitBlt`; clear, and the rectangle is just filled with the
 * parent's brush -- or left alone if there is no brush.
 *
 * THE WINDOW LONG IS FETCHED TWICE, once to test and once to keep, with the same two
 * arguments. `GetWindowLong(parent, 0)` is fetched again in the other half as well.
 *
 * The source point is the damaged rectangle's own corner, mapped from this window into the
 * port's window by `MapWindowPoints` -- so the blit reads the part of the parent's bitmap that
 * lies under this child, which is what makes a child of a WinG window free to be anywhere.
 *
 * The palette is only selected when the port has one AND DGROUP:0x3380 is exactly 1: a
 * palette-mode flag, not a boolean, and 2 or 3 would skip it.
 *
 * `ps` is a PAINTSTRUCT: the DC at +0 and the damaged rectangle at +4, which is why the width
 * and height helpers are handed `(char *)ps + 4`.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

/* same object: 4-byte `push cs; call ` */

void wing_child_paint(PAINTSTRUCT *ps, register unsigned hwnd)
{
    POINT pt;
    MSWin *parent_win;
    void *gp;
    GrafRec *graf;
    register unsigned parent;

    parent = GetParent(hwnd);
    if (GetWindowLong(parent, 12) != 0) {
        gp = (void *)GetWindowLong(parent, 12);
        parent_win = get_win_1070_95f0((Screen *)((void *)GetWindowLong(parent, 0)));
        graf = GRPort_GetWin_1070_960d((GRPort *)gp);
        pt.x = ps->rcPaint.left;
        pt.y = ps->rcPaint.top;
        MapWindowPoints(hwnd, parent_win->hwnd, &pt, 1);
        if (parent_win->hpal != 0 && g_palette_mode == 1) {
            SelectPalette(ps->hdc, parent_win->hpal, 0);
            RealizePalette(ps->hdc);
        }
        WinGBitBlt(ps->hdc, ps->rcPaint.left, ps->rcPaint.top,
                   rect_width_1070_962a(&ps->rcPaint),
                   rect_height_1070_9649(&ps->rcPaint),
                   graf->hdc, pt.x, pt.y);
    } else {
        parent_win = get_win_1070_95f0((Screen *)((void *)GetWindowLong(parent, 0)));
        if (parent_win->hbrush != 0)
            FillRect(ps->hdc, &ps->rcPaint, parent_win->hbrush);
    }
}
