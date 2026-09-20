/* is_state_one -- shipped at 1070:a70f.
 *
 * Ported by tools/port.py from matched/platform/is_state_one.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: state -> v_a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 is_state_one(Rec *r)
{
    if (r->v_a == 1)
        return 1;
    else
        return 0;
}
