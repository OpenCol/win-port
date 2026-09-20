/* get_w_a -- shipped at 1008:d0fe.
 *
 * Ported by tools/port.py from matched/game/get_w_a.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_a(Rec *r)
{
    return r->v_a;
}
