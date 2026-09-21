/* listbox_delete_string -- shipped at 1070:5433.
 *
 * Ported by tools/port.py from matched/platform/listbox_delete_string.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal SendMessage(col_u16 hwnd, col_u16 msg, col_u16 wp, col_i32 lp);
#endif

void listbox_delete_string(col_u16 hwnd, col_i16 i)
{
    SendMessage(hwnd, 0x403, i, 0L);
}
