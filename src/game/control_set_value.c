/* control_set_value -- shipped at 1060:66e9.
 *
 * Ported by tools/port.py from matched/game/control_set_value.c.
 *
 * The file's own Control dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void scroll_set_pos(col_i16 h, col_i16 v);

void control_set_value(Control *c, col_i16 v)
{
    c->value = v;
    scroll_set_pos(c->hwnd, v);
}
