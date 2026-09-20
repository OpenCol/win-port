/* get_w_4_1008_d720 -- shipped at 1008:d720.
 *
 * Ported by tools/port.py from matched/game/get_w_4_1008_d720.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_4_1008_d720(Rec *r)
{
    return r->handled;
}
