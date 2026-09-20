/* byte_at_16 -- shipped at 1060:ac00.
 *
 * Ported by tools/port.py from matched/game/byte_at_16.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

char byte_at_16(Rec *r)
{
    return r->f16;
}
