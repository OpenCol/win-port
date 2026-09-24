/* ctl_id_1070_96c5 -- shipped at 1070:96c5.
 *
 * Ported by tools/port.py from matched/platform/ctl_id_1070_96c5.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_2
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 ctl_id_1070_96c5(Rec *r)
{
    return r->v_2;
}
