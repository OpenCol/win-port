/* win_client_height_1078_241a -- shipped at 1078:241a.
 *
 * Ported by tools/port.py from matched/platform/win_client_height_1078_241a.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal GetClientRect(col_u16 hwnd, RECT *r);;
#endif

col_i16 win_client_height_1078_241a(MSWin *w)
{
    RECT r;

    if (w == 0)
        return 0;
    GetClientRect(w->hwnd, &r);
    return r.bottom;
}
