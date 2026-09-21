/* set_long_12 -- shipped at 1078:1f41.
 *
 * Ported by tools/port.py from matched/platform/set_long_12.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_12
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_long_12(Rec *r, col_i32 v)
{
    r->v_12 = v;
}
