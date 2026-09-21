/* mode_dispatch -- shipped at 1010:e145.
 *
 * Ported by tools/port.py from matched/game/mode_dispatch.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_colony_mode;              DGROUP:0x0c0f
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void empty_1010_e0f1(void);
void empty_1010_e129(void);
void empty_1010_e137(void);

/* same object: 4-byte `push cs; call ` (rule 47) */


void mode_dispatch(void)
{
    switch (g_colony_mode) {
    case 2:
        empty_1010_e129();
        break;
    case 1:
        empty_1010_e0f1();
        break;
    case 0:
        empty_1010_e137();
        break;
    }
}
