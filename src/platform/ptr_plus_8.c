/* ptr_plus_8 -- shipped at 1070:9704.
 *
 * Ported by tools/port.py from matched/platform/ptr_plus_8.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * ptr_plus_8(char *p)
{
    return p + 8;
}
