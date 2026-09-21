/* MSWaitTicks_1010_e5f6 -- shipped at 1010:e5f6.
 *
 * Ported by tools/port.py from matched/game/MSWaitTicks_1010_e5f6.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSWaitTicks(col_i16 a);

void MSWaitTicks_1010_e5f6(col_i16 a)
{
    MSWaitTicks(a);
}
