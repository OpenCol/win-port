/* MSWaitTicks_1068_d93a -- shipped at 1068:d93a.
 *
 * Ported by tools/port.py from matched/gfx/MSWaitTicks_1068_d93a.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSWaitTicks(col_i16 a);

void MSWaitTicks_1068_d93a(col_i16 a)
{
    MSWaitTicks(a);
}
