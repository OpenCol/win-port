/* set_pair_6e_1038_e1d5 -- shipped at 1038:e1d5.
 *
 * Ported by tools/port.py from matched/game/set_pair_6e_1038_e1d5.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_pair_6e_1038_e1d5(Rec *r, col_i16 x, col_i16 y)
{
    r->x = x;
    r->y = y;
}
