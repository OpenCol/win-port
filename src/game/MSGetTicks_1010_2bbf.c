/* MSGetTicks_1010_2bbf -- shipped at 1010:2bbf.
 *
 * Ported by tools/port.py from matched/game/MSGetTicks_1010_2bbf.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i32 MSGetTicks(void);

col_i32 MSGetTicks_1010_2bbf(void)
{
    return MSGetTicks();
}
