/* MSWaitTicks_1048_dbd6 -- shipped at 1048:dbd6.
 *
 * Ported by tools/port.py from matched/game/MSWaitTicks_1048_dbd6.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSWaitTicks(col_i16 a);

void MSWaitTicks_1048_dbd6(col_i16 a)
{
    MSWaitTicks(a);
}
