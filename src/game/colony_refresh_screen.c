/* colony_refresh_screen -- shipped at 1018:8c7c.
 *
 * Ported by tools/port.py from matched/game/colony_refresh_screen.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a, g_stub_b;                       
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void colony_rebuild_screen(void);
void colony_refresh_all(void);

void colony_refresh_screen(void)
{
    colony_rebuild_screen();
    colony_refresh_all();
}
