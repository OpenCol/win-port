/* show_hwnd -- shipped at 1070:0b43.
 *
 * Ported by tools/port.py from matched/platform/show_hwnd.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal ShowWindow(col_i16 h, col_i16 cmd);;
#endif

void show_hwnd(col_i16 h)
{
    if (h == 0)
        return;
    ShowWindow(h, 5);
}
