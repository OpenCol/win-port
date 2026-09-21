/* set_focus -- shipped at 1070:0593.
 *
 * Ported by tools/port.py from matched/platform/set_focus.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal SetFocus(col_u16 hwnd);
#endif

void set_focus(col_u16 hwnd)
{
    SetFocus(hwnd);
}
