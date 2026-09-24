/* seed_random -- shipped at 1040:7508.
 *
 * Ported by tools/port.py from matched/game/seed_random.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void seed_random_from_ticks(void);

/* same object: 4-byte `push cs; call ` */


void seed_random(col_i16 seed)
{
    seed_random_from_ticks();
}
