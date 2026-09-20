/* get_w_2_1040_abae -- shipped at 1040:abae.
 *
 * Ported by tools/port.py from matched/game/get_w_2_1040_abae.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_2
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_2_1040_abae(Rec *r)
{
    return r->v_2;
}
