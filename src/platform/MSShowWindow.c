/* MSShowWindow -- shipped at 1078:1f75.
 *
 * Ported by tools/port.py from matched/platform/MSShowWindow.c.
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
extern void pascal BringWindowToTop(col_u16 hwnd);;
extern void pascal ShowWindow(col_u16 hwnd, col_i16 cmd);;
#endif

void MSShowWindow(Win *w)
{
    if (w != 0) {
        ShowWindow(w->hdc, 5);
        if (w->flags & 2)
            BringWindowToTop(w->hdc);
    }
}
