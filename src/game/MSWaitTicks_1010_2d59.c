/* MSWaitTicks_1010_2d59 -- shipped at 1010:2d59.
 *
 * Ported by tools/port.py from matched/game/MSWaitTicks_1010_2d59.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSWaitTicks(col_i16 a);

void MSWaitTicks_1010_2d59(col_i16 a)
{
    MSWaitTicks(a);
}
