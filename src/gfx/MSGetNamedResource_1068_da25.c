/* MSGetNamedResource_1068_da25 -- shipped at 1068:da25.
 *
 * Ported by tools/port.py from matched/gfx/MSGetNamedResource_1068_da25.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSGetNamedResource(void *type, col_i16 id);

void * MSGetNamedResource_1068_da25(void *type, col_i16 id)
{
    return MSGetNamedResource(type, id);
}
