/* colony_ring_tiles -- shipped at 1018:3f45.
 *
 * Ported by tools/port.py from matched/game/colony_ring_tiles.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 colony_ring_level(void);

col_i16 colony_ring_tiles(void)
{
    register col_i16 r;

    switch (colony_ring_level()) {
    case 1:
        r = 4;
        break;
    case 2:
        r = 8;
        break;
    case 3:
        r = 12;
        break;
    default:
        r = 0x20;
        break;
    }
    return r;
}
