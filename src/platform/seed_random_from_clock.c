/* seed_random_from_clock -- shipped at 1070:dd1d.
 *
 * Ported by tools/port.py from matched/platform/seed_random_from_clock.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i32 time_rtl(col_i32 *t);
extern void srand_rtl(col_u16 seed);

void seed_random_from_clock(void)
{
    srand_rtl((col_u16)time_rtl(0L));
}
