/* win_get_ctl -- shipped at 1070:65aa.
 *
 * Ported by tools/port.py from matched/platform/win_get_ctl.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal GetWindowLong(col_u16 hwnd, col_i16 index);
#endif

col_i32 win_get_ctl(col_u16 hwnd)
{
    return GetWindowLong(hwnd, 6);
}
