/* invalidate_hwnd -- shipped at 1070:0b1c.
 *
 * Ported by tools/port.py from matched/platform/invalidate_hwnd.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal InvalidateRect(col_i16 h, void *r, col_i16 erase);;
#endif

void invalidate_hwnd(col_i16 h)
{
    if (h == 0)
        return;
    InvalidateRect(h, 0, 0);
}
