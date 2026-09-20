/* get_w_2_1040_44cf -- shipped at 1040:44cf.
 *
 * Ported by tools/port.py from matched/game/get_w_2_1040_44cf.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_2
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_2_1040_44cf(Rec *r)
{
    return r->v_2;
}
