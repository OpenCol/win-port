/* get_w_1a_1060_ac31 -- shipped at 1060:ac31.
 *
 * Ported by tools/port.py from matched/game/get_w_1a_1060_ac31.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_1a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_1a_1060_ac31(Rec *r)
{
    return r->v_1a;
}
