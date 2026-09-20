/* get_w_7a -- shipped at 1070:a032.
 *
 * Ported by tools/port.py from matched/platform/get_w_7a.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: width -> width_7a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_7a(Rec *r)
{
    return r->width_7a;
}
