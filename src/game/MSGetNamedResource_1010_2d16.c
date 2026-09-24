/* MSGetNamedResource_1010_2d16 -- shipped at 1010:2d16.
 *
 * Ported by tools/port.py from matched/game/MSGetNamedResource_1010_2d16.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSGetNamedResource(void *type, col_i16 id);

void * MSGetNamedResource_1010_2d16(void *type, col_i16 id)
{
    return MSGetNamedResource(type, id);
}
