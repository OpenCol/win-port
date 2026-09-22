/* listbox_get_text_1070_54b4 -- shipped at 1070:54b4.
 *
 * Ported by tools/port.py from matched/platform/listbox_get_text_1070_54b4.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal SendMessage(col_u16 hwnd, col_u16 msg, col_u16 wp, col_i32 lp);
#endif

void listbox_get_text_1070_54b4(col_u16 hwnd, col_i16 i, char *buf)
{
    SendMessage(hwnd, 0x40a, i, (col_i32)buf);
}
