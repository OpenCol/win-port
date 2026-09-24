/* get_w_0 -- shipped at 1060:afce.
 *
 * Ported by tools/port.py from matched/game/get_w_0.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_0(Rec *r)
{
    return r->v;
}
