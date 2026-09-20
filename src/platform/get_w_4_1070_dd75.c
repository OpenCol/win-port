/* get_w_4_1070_dd75 -- shipped at 1070:dd75.
 *
 * Ported by tools/port.py from matched/platform/get_w_4_1070_dd75.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_4_1070_dd75(Rec *r)
{
    return r->handled;
}
