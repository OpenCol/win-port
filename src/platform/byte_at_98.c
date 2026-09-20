/* byte_at_98 -- shipped at 1070:9889.
 *
 * Ported by tools/port.py from matched/platform/byte_at_98.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: f98 -> v_98
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

char byte_at_98(Rec *r)
{
    return r->v_98;
}
