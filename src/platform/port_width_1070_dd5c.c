/* port_width_1070_dd5c -- shipped at 1070:dd5c.
 *
 * Ported by tools/port.py from matched/platform/port_width_1070_dd5c.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_2
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 port_width_1070_dd5c(Rec *r)
{
    return r->v_2;
}
