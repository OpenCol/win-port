/* get_w_88 -- shipped at 1070:9f4d.
 *
 * Ported by tools/port.py from matched/platform/get_w_88.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_88
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_88(Rec *r)
{
    return r->v_88;
}
