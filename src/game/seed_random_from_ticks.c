/* seed_random_from_ticks -- shipped at 1040:74cd.
 *
 * Ported by tools/port.py from matched/game/seed_random_from_ticks.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i32 MSGetTicks_1040_aaa0(void);

extern void srand_rtl(col_u16 seed);

/* same object: 4-byte `push cs; call ` */


void seed_random_from_ticks(void)
{
    srand_rtl((col_u16)MSGetTicks_1040_aaa0() & 0x7fff);
}
