/* set_pair_72_1050_0d1f -- shipped at 1050:0d1f.
 *
 * Ported by tools/port.py from matched/game/set_pair_72_1050_0d1f.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: x -> x_72, y -> y_74
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_pair_72_1050_0d1f(Rec *r, col_i16 x, col_i16 y)
{
    r->x_72 = x;
    r->y_74 = y;
}
