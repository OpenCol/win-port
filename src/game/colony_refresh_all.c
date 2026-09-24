/* colony_refresh_all -- shipped at 1018:8c66.
 *
 * Ported by tools/port.py from matched/game/colony_refresh_all.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a, g_stub_b;                       
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void colony_production(void);
void count_tile_units(void);

void colony_refresh_all(void)
{
    count_tile_units();
    colony_production();
}
