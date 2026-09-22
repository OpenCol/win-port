/* listbox_add_string -- shipped at 1070:5414.
 *
 * Ported by tools/port.py from matched/platform/listbox_add_string.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal SendMessage(col_u16 hwnd, col_u16 msg, col_u16 wp, col_i32 lp);
#endif

void listbox_add_string(col_u16 hwnd, const char *s)
{
    SendMessage(hwnd, 0x401, 0, (col_i32)s);
}
