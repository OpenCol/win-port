/* get_w_20_1070_997b -- shipped at 1070:997b.
 *
 * Ported by tools/port.py from matched/platform/get_w_20_1070_997b.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_20
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_20_1070_997b(Rec *r)
{
    return r->v_20;
}
