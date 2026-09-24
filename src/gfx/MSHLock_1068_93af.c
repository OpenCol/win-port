/* MSHLock_1068_93af -- shipped at 1068:93af.
 *
 * Ported by tools/port.py from matched/gfx/MSHLock_1068_93af.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSHLock(col_i16 h);

void * MSHLock_1068_93af(col_i16 a)
{
    return MSHLock(a);
}
