/* get_w_2_1068_e517 -- shipped at 1068:e517.
 *
 * Ported by tools/port.py from matched/gfx/get_w_2_1068_e517.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_2
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_2_1068_e517(Rec *r)
{
    return r->v_2;
}
