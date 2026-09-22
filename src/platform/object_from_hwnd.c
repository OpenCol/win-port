/* object_from_hwnd -- shipped at 1070:09ae.
 *
 * Ported by tools/port.py from matched/platform/object_from_hwnd.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal GetWindowLong(col_u16 hwnd, col_i16 index);
#endif

void * object_from_hwnd(col_u16 hwnd)
{
    if (hwnd != 0)
        return (void *)GetWindowLong(hwnd, 0);
    else
        return 0;
}
