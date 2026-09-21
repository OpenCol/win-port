/* MSGetTicks_1068_d953 -- shipped at 1068:d953.
 *
 * Ported by tools/port.py from matched/gfx/MSGetTicks_1068_d953.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i32 MSGetTicks(void);

col_i32 MSGetTicks_1068_d953(void)
{
    return MSGetTicks();
}
