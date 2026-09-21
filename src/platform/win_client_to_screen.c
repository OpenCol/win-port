/* win_client_to_screen -- shipped at 1078:2593.
 *
 * Ported by tools/port.py from matched/platform/win_client_to_screen.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: hwnd -> hdc
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal ClientToScreen(col_u16 hwnd, POINT *p);;
#endif

void win_client_to_screen(Win *w, col_i16 x, col_i16 y, col_i16 *px, col_i16 *py)
{
    POINT pt;

    if (w == 0)
        return;
    pt.x = x;
    pt.y = y;
    ClientToScreen(w->hdc, &pt);
    *px = pt.x;
    *py = pt.y;
}
