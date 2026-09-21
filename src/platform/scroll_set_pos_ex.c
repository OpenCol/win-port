/* scroll_set_pos_ex -- shipped at 1070:6081.
 *
 * Ported by tools/port.py from matched/platform/scroll_set_pos_ex.c.
 *
 * The file's own Ctl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal SetScrollPos(col_i16 h, col_i16 bar, col_i16 pos, col_i16 redraw);
#endif

void scroll_set_pos_ex(Ctl *c, col_i16 pos, col_i16 flag)
{
    SetScrollPos(c->hwnd, flag != 0, pos, 1);
}
