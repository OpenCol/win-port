/* get_w_4_1048_d86c -- shipped at 1048:d86c.
 *
 * Ported by tools/port.py from matched/game/get_w_4_1048_d86c.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_4_1048_d86c(Rec *r)
{
    return r->handled;
}
