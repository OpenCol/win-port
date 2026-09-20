/* get_w_10_1060_ac4a -- shipped at 1060:ac4a.
 *
 * Ported by tools/port.py from matched/game/get_w_10_1060_ac4a.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_10
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_10_1060_ac4a(Rec *r)
{
    return r->v_10;
}
