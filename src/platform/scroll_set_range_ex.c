/* scroll_set_range_ex -- shipped at 1070:6007.
 *
 * Ported by tools/port.py from matched/platform/scroll_set_range_ex.c.
 *
 * The file's own Ctl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetScrollRange(col_i16 h, col_i16 bar, col_i16 lo, col_i16 hi, col_i16 redraw);;
#endif

extern void scroll_set_pos_ex(Ctl *c, col_i16 pos, col_i16 flag);

void scroll_set_range_ex(Ctl *c, col_i16 lo, col_i16 hi, col_i16 flag)
{
    SetScrollRange(c->hwnd, flag != 0, lo, hi, 1);
    scroll_set_pos_ex(c, lo, flag);
}
