/* MSGetNamedResource_1060_8884 -- shipped at 1060:8884.
 *
 * Ported by tools/port.py from matched/game/MSGetNamedResource_1060_8884.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSGetNamedResource(void *type, col_i16 id);

void * MSGetNamedResource_1060_8884(void *type, col_i16 id)
{
    return MSGetNamedResource(type, id);
}
