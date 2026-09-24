/* ctl_scroll_range -- shipped at 1060:66c6.
 *
 * Ported by tools/port.py from matched/game/ctl_scroll_range.c.
 *
 * The file's own DlgControl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void scroll_set_range(col_i16 h, col_i16 lo, col_i16 hi);

void ctl_scroll_range(DlgControl *c, col_i16 a, col_i16 b)
{
    scroll_set_range(c->hwnd, a, b);
}
