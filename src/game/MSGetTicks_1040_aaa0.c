/* MSGetTicks_1040_aaa0 -- shipped at 1040:aaa0.
 *
 * Ported by tools/port.py from matched/game/MSGetTicks_1040_aaa0.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i32 MSGetTicks(void);

col_i32 MSGetTicks_1040_aaa0(void)
{
    return MSGetTicks();
}
