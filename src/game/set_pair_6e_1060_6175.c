/* set_pair_6e_1060_6175 -- shipped at 1060:6175.
 *
 * Ported by tools/port.py from matched/game/set_pair_6e_1060_6175.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_pair_6e_1060_6175(Rec *r, col_i16 x, col_i16 y)
{
    r->x = x;
    r->y = y;
}
