/* get_w_26_1070_9ada -- shipped at 1070:9ada.
 *
 * Ported by tools/port.py from matched/platform/get_w_26_1070_9ada.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_26
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_26_1070_9ada(Rec *r)
{
    return r->v_26;
}
