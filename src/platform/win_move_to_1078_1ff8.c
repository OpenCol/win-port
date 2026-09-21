/* win_move_to_1078_1ff8 -- shipped at 1078:1ff8.
 *
 * Ported by tools/port.py from matched/platform/win_move_to_1078_1ff8.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_height_1078_3418(RECT *r);
col_i16 rect_width_1078_33f9(RECT *r);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal GetWindowRect(col_u16 hwnd, RECT *r);
extern col_i16 pascal MoveWindow(col_u16 hwnd, col_i16 x, col_i16 y, col_i16 w, col_i16 h, col_i16 repaint);
#endif

void win_move_to_1078_1ff8(MSWin *w, col_i16 x, col_i16 y)
{
    RECT r;
    if (w == 0)
        return;
    GetWindowRect(w->hwnd, &r);
    MoveWindow(w->hwnd, x, y, rect_width_1078_33f9(&r), rect_height_1078_3418(&r), 1);
}
