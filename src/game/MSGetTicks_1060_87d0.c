/* MSGetTicks_1060_87d0 -- shipped at 1060:87d0.
 *
 * Ported by tools/port.py from matched/game/MSGetTicks_1060_87d0.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i32 MSGetTicks(void);

col_i32 MSGetTicks_1060_87d0(void)
{
    return MSGetTicks();
}
