/* MSWaitTicks_1070_984a -- shipped at 1070:984a.
 *
 * Ported by tools/port.py from matched/platform/MSWaitTicks_1070_984a.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSWaitTicks(col_i16 a);

void MSWaitTicks_1070_984a(col_i16 a)
{
    MSWaitTicks(a);
}
