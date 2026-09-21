/* MSWaitTicks_1020_a9c5 -- shipped at 1020:a9c5.
 *
 * Ported by tools/port.py from matched/game/MSWaitTicks_1020_a9c5.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSWaitTicks(col_i16 a);

void MSWaitTicks_1020_a9c5(col_i16 a)
{
    MSWaitTicks(a);
}
