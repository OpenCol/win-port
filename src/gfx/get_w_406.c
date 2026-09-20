/* get_w_406 -- shipped at 1068:98ca.
 *
 * Ported by tools/port.py from matched/gfx/get_w_406.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_406
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_406(Rec *r)
{
    return r->v_406;
}
