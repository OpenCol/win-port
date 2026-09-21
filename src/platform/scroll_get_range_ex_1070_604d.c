/* scroll_get_range_ex_1070_604d -- shipped at 1070:604d.
 *
 * Ported by tools/port.py from matched/platform/scroll_get_range_ex_1070_604d.c.
 *
 * The file's own Ctl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal GetScrollRange(col_i16 h, col_i16 bar, col_i16 *lo, col_i16 *hi);
#endif

void scroll_get_range_ex_1070_604d(Ctl *c, col_i16 *lo, col_i16 *hi, col_i16 flag)
{
    GetScrollRange(c->hwnd, flag != 0, lo, hi);
}
