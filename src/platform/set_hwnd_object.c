/* set_hwnd_object -- shipped at 1070:67bd.
 *
 * Ported by tools/port.py from matched/platform/set_hwnd_object.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal SetWindowLong(col_u16 hwnd, col_i16 index, col_i32 value);
#endif

/* the HWND is at +2 */


void set_hwnd_object(void *obj, MSWin *w)
{
    SetWindowLong(w->hwnd, 0, (col_i32)obj);
}
