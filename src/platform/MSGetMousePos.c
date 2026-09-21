/* MSGetMousePos -- shipped at 1078:2c91.
 *
 * Ported by tools/port.py from matched/platform/MSGetMousePos.c.
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
extern void pascal GetCursorPos(POINT *p);
extern void pascal ScreenToClient(col_u16 hwnd, POINT *p);
#endif

void MSGetMousePos(Win *w, col_i16 *x, col_i16 *y)
{
    POINT p;

    GetCursorPos(&p);
    ScreenToClient(w->hdc, &p);
    *x = p.x;
    *y = p.y;
}
