/* set_w_2a_1070_9994 -- shipped at 1070:9994.
 *
 * Ported by tools/port.py from matched/platform/set_w_2a_1070_9994.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_2a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_w_2a_1070_9994(Rec *r, col_i16 v)
{
    r->v_2a = v;
}
