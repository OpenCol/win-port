/* set_w_1e_1050_87a2 -- shipped at 1050:87a2.
 *
 * Ported by tools/port.py from matched/game/set_w_1e_1050_87a2.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_1e
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_w_1e_1050_87a2(Rec *r, col_i16 v)
{
    r->v_1e = v;
}
