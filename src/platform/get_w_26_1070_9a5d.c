/* get_w_26_1070_9a5d -- shipped at 1070:9a5d.
 *
 * Ported by tools/port.py from matched/platform/get_w_26_1070_9a5d.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_26
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_26_1070_9a5d(Rec *r)
{
    return r->v_26;
}
