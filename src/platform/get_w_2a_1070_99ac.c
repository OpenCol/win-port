/* get_w_2a_1070_99ac -- shipped at 1070:99ac.
 *
 * Ported by tools/port.py from matched/platform/get_w_2a_1070_99ac.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_2a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_2a_1070_99ac(Rec *r)
{
    return r->v_2a;
}
