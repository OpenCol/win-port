/* get_w_6_1048_e56c -- shipped at 1048:e56c.
 *
 * Ported by tools/port.py from matched/game/get_w_6_1048_e56c.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_6
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_6_1048_e56c(Rec *r)
{
    return r->v_6;
}
