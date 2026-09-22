/* get_p_12_1060_abe3 -- shipped at 1060:abe3.
 *
 * Ported by tools/port.py from matched/game/get_p_12_1060_abe3.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_12
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_12_1060_abe3(Rec *r)
{
    return r->v_12;
}
