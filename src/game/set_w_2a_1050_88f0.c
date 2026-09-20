/* set_w_2a_1050_88f0 -- shipped at 1050:88f0.
 *
 * Ported by tools/port.py from matched/game/set_w_2a_1050_88f0.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_2a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_w_2a_1050_88f0(Rec *r, col_i16 v)
{
    r->v_2a = v;
}
