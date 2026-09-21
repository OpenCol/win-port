/* MSGetTicks_1068_990a -- shipped at 1068:990a.
 *
 * Ported by tools/port.py from matched/gfx/MSGetTicks_1068_990a.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i32 MSGetTicks(void);

col_i32 MSGetTicks_1068_990a(void)
{
    return MSGetTicks();
}
