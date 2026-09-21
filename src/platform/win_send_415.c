/* win_send_415 -- shipped at 1070:5adc.
 *
 * Ported by tools/port.py from matched/platform/win_send_415.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal SendMessage(col_u16 hwnd, col_u16 msg, col_u16 wp, col_i32 lp);
#endif

void win_send_415(col_u16 hwnd, col_u16 wp)
{
    SendMessage(hwnd, 0x415, wp, 0L);
}
