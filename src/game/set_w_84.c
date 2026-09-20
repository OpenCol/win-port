/* set_w_84 -- shipped at 1050:0bc0.
 *
 * Ported by tools/port.py from matched/game/set_w_84.c.
 *
 * The file's own Popup dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_w_84(Popup *p, col_i16 v)
{
    p->w84 = v;
}
