/* MSGetNamedResource_1068_94c7 -- shipped at 1068:94c7.
 *
 * Ported by tools/port.py from matched/gfx/MSGetNamedResource_1068_94c7.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSGetNamedResource(void *type, col_i16 id);

void * MSGetNamedResource_1068_94c7(void *type, col_i16 id)
{
    return MSGetNamedResource(type, id);
}
