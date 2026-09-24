/* port_height_1050_9247 -- shipped at 1050:9247.
 *
 * Ported by tools/port.py from matched/game/port_height_1050_9247.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> handled
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 port_height_1050_9247(Rec *r)
{
    return r->handled;
}
