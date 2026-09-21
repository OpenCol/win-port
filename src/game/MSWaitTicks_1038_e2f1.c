/* MSWaitTicks_1038_e2f1 -- shipped at 1038:e2f1.
 *
 * Ported by tools/port.py from matched/game/MSWaitTicks_1038_e2f1.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSWaitTicks(col_i16 a);

void MSWaitTicks_1038_e2f1(col_i16 a)
{
    MSWaitTicks(a);
}
