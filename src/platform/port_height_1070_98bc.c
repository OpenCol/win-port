/* port_height_1070_98bc -- shipped at 1070:98bc.
 *
 * Ported by tools/port.py from matched/platform/port_height_1070_98bc.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> handled
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 port_height_1070_98bc(Rec *r)
{
    return r->handled;
}
