/* get_w_2_1060_65ca -- shipped at 1060:65ca.
 *
 * Ported by tools/port.py from matched/game/get_w_2_1060_65ca.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_2
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_2_1060_65ca(Rec *r)
{
    return r->v_2;
}
