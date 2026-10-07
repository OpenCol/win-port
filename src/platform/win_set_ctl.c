/* win_set_ctl -- shipped at 1070:65c6.
 *
 * Ported by tools/port.py from matched/platform/win_set_ctl.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal SetWindowLong(col_u16 hwnd, col_i16 index, col_i32 value);
#endif

void win_set_ctl(col_u16 hwnd, col_i32 ctl)
{
    SetWindowLong(hwnd, 6, ctl);
}
