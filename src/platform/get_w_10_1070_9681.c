/* get_w_10_1070_9681 -- shipped at 1070:9681.
 *
 * Ported by tools/port.py from matched/platform/get_w_10_1070_9681.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_10
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_10_1070_9681(Rec *r)
{
    return r->v_10;
}
