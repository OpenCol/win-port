/* scroll_set_range -- shipped at 1070:5f8f.
 *
 * Ported by tools/port.py from matched/platform/scroll_set_range.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetScrollRange(col_i16 h, col_i16 bar, col_i16 lo, col_i16 hi, col_i16 redraw);
#endif

extern void scroll_set_pos(col_i16 h, col_i16 v);

void scroll_set_range(col_i16 h, col_i16 lo, col_i16 hi)
{
    SetScrollRange(h, 2, lo, hi, 1);
    scroll_set_pos(h, lo);
}
