/* get_w_26_1060_ac99 -- shipped at 1060:ac99.
 *
 * Ported by tools/port.py from matched/game/get_w_26_1060_ac99.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_26
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_26_1060_ac99(Rec *r)
{
    return r->v_26;
}
