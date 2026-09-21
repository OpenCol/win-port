/* next_sibling_window -- shipped at 1070:8fc2.
 *
 * Ported by tools/port.py from matched/platform/next_sibling_window.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal GetNextWindow(col_i16 h, col_i16 flag);;
extern col_i16 pascal GetWindow(col_i16 h, col_i16 cmd);;
extern col_i16 pascal IsWindow(col_i16 h);;
#endif

col_i16 next_sibling_window(col_i16 h)
{
    col_i16 w;

    if (IsWindow(h) == 0)
        return 0;
    w = GetNextWindow(h, 2);
    if (w == 0)
        w = GetWindow(h, 0);
    return w;
}
