/* get_w_6_1068_d7db -- shipped at 1068:d7db.
 *
 * Ported by tools/port.py from matched/gfx/get_w_6_1068_d7db.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_6
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_6_1068_d7db(Rec *r)
{
    return r->v_6;
}
