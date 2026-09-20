/* ptr_plus_16 -- shipped at 1068:d747.
 *
 * Ported by tools/port.py from matched/gfx/ptr_plus_16.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * ptr_plus_16(char *p)
{
    return p + 0x10;
}
