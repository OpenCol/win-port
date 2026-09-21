/* win_set_back_color_1078_1ed6 -- shipped at 1078:1ed6.
 *
 * Ported by tools/port.py from matched/platform/win_set_back_color_1078_1ed6.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: hbrush -> f8
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal DeleteObject(col_u16 h);
extern col_i16 pascal InvalidateRect(col_u16 hwnd, void *r, col_i16 erase);
extern col_u16 pascal CreateSolidBrush(col_u32 color);
#endif

void win_set_back_color_1078_1ed6(MSWin *w, char index)
{
    if (w->f8 != 0)
        DeleteObject(w->f8);
    w->f8 = CreateSolidBrush(0x01000000L | (col_u32)(col_u16)index);
    InvalidateRect(w->hwnd, 0, 0);
}
