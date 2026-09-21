/* listbox_reset_content -- shipped at 1070:5496.
 *
 * Ported by tools/port.py from matched/platform/listbox_reset_content.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal SendMessage(col_u16 hwnd, col_u16 msg, col_u16 wp, col_i32 lp);
#endif

void listbox_reset_content(col_u16 hwnd)
{
    SendMessage(hwnd, 0x405, 0, 0L);
}
