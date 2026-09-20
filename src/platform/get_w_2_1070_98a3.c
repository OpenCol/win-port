/* get_w_2_1070_98a3 -- shipped at 1070:98a3.
 *
 * Ported by tools/port.py from matched/platform/get_w_2_1070_98a3.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_2
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_2_1070_98a3(Rec *r)
{
    return r->v_2;
}
