/* scroll_get_range -- shipped at 1070:5fbf.
 *
 * Ported by tools/port.py from matched/platform/scroll_get_range.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal GetScrollRange(col_i16 h, col_i16 bar, col_i16 *lo, col_i16 *hi);;
#endif

void scroll_get_range(col_i16 h, col_i16 *lo, col_i16 *hi)
{
    GetScrollRange(h, 2, lo, hi);
}
