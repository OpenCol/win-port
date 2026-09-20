/* get_w_2a_1050_848f -- shipped at 1050:848f.
 *
 * Ported by tools/port.py from matched/game/get_w_2a_1050_848f.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_2a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_2a_1050_848f(Rec *r)
{
    return r->v_2a;
}
