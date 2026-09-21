/* enable_window -- shipped at 1070:5f76.
 *
 * Ported by tools/port.py from matched/platform/enable_window.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal EnableWindow(col_u16 hwnd, col_i16 on);;
#endif

void enable_window(col_u16 hwnd, col_i16 on)
{
    EnableWindow(hwnd, on);
}
