/* MSSetFocus -- shipped at 1078:2a30.
 *
 * Ported by tools/port.py from matched/platform/MSSetFocus.c.
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
extern col_u16 pascal SetFocus(col_u16 hwnd);;
extern void pascal BringWindowToTop(col_u16 hwnd);;
#endif

void MSSetFocus(Win *w)
{
    if (w == 0)
        return;
    SetFocus(w->hdc);
    BringWindowToTop(w->hdc);
}
