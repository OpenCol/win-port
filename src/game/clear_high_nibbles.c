/* clear_high_nibbles -- shipped at 1040:4275.
 *
 * Ported by tools/port.py from matched/game/clear_high_nibbles.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_map_width;                          DGROUP:0x4c40
 *   extern int far g_map_h;                              DGROUP:0x4c42
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void map_plane2_set_high_nibble(col_i16 x, col_i16 y, col_i16 v);

void clear_high_nibbles(void)
{
    register col_i16 x;
    register col_i16 y;

    for (y = 0; g_map_h > y; y++) {
        for (x = 0; (*g_map_width) > x; x++)
            map_plane2_set_high_nibble(x, y, -1);
    }
}
