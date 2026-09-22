/* get_p_12_1070_a09a -- shipped at 1070:a09a.
 *
 * Ported by tools/port.py from matched/platform/get_p_12_1070_a09a.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_12
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_12_1070_a09a(Rec *r)
{
    return r->v_12;
}
