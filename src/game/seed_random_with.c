/* seed_random_with -- shipped at 1040:74eb.
 *
 * Ported by tools/port.py from matched/game/seed_random_with.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void srand_rtl(col_u16 seed);

void seed_random_with(col_i32 seed)
{
    srand_rtl((col_u16)seed & 0x7fff);
}
