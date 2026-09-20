/* get_w_4_1010_ebed -- shipped at 1010:ebed.
 *
 * Ported by tools/port.py from matched/game/get_w_4_1010_ebed.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_4_1010_ebed(Rec *r)
{
    return r->handled;
}
