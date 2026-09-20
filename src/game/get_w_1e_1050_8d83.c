/* get_w_1e_1050_8d83 -- shipped at 1050:8d83.
 *
 * Ported by tools/port.py from matched/game/get_w_1e_1050_8d83.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_1e
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_1e_1050_8d83(Rec *r)
{
    return r->v_1e;
}
