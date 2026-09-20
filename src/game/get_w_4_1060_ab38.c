/* get_w_4_1060_ab38 -- shipped at 1060:ab38.
 *
 * Ported by tools/port.py from matched/game/get_w_4_1060_ab38.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_4_1060_ab38(Rec *r)
{
    return r->handled;
}
