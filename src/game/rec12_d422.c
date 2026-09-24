/* rec12_d422 -- shipped at 1018:8c92.
 *
 * Ported by tools/port.py from matched/game/rec12_d422.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void colony_site_layout(void);

/* same object: 4-byte `push cs; call ` */


void rec12_d422(void)
{
    colony_site_layout();
}
