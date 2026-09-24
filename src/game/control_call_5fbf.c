/* control_call_5fbf -- shipped at 1060:aa1e.
 *
 * Ported by tools/port.py from matched/game/control_call_5fbf.c.
 *
 * The file's own DlgControl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void scroll_get_range(col_i16 h, col_i16 a, col_i16 b, col_i16 c, col_i16 d);

void control_call_5fbf(DlgControl *c, col_i16 a, col_i16 b, col_i16 d, col_i16 e)
{
    scroll_get_range(c->hwnd, a, b, d, e);
}
