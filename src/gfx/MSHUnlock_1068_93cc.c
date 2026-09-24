/* MSHUnlock_1068_93cc -- shipped at 1068:93cc.
 *
 * Ported by tools/port.py from matched/gfx/MSHUnlock_1068_93cc.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSHUnlock(col_i16 h);

void * MSHUnlock_1068_93cc(col_i16 a)
{
    return MSHUnlock(a);
}
