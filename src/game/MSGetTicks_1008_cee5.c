/* MSGetTicks_1008_cee5 -- shipped at 1008:cee5.
 *
 * Ported by tools/port.py from matched/game/MSGetTicks_1008_cee5.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i32 MSGetTicks(void);

col_i32 MSGetTicks_1008_cee5(void)
{
    return MSGetTicks();
}
