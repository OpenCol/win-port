/* ctl_swap_state_byte -- shipped at 1050:1706.
 *
 * Ported by tools/port.py from matched/game/ctl_swap_state_byte.c.
 *
 * The file's own Ctl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern char TextColor(char v);

void ctl_swap_state_byte(Ctl *c, col_i16 a, col_i16 b)
{
    if (a)
        TextColor(c->s94);
    else if (b)
        TextColor(c->s96);
    else
        TextColor(c->s92);
}
