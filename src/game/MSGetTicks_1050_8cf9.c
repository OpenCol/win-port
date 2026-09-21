/* MSGetTicks_1050_8cf9 -- shipped at 1050:8cf9.
 *
 * Ported by tools/port.py from matched/game/MSGetTicks_1050_8cf9.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i32 MSGetTicks(void);

col_i32 MSGetTicks_1050_8cf9(void)
{
    return MSGetTicks();
}
