/* MSDrawMenuBar -- shipped at 1070:d63e.
 *
 * Ported by tools/port.py from matched/platform/MSDrawMenuBar.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal DrawMenuBar(col_u16 hwnd);;
#endif

/* the HWND is at +2 */


void MSDrawMenuBar(MSWin *w)
{
    DrawMenuBar(w->hwnd);
}
