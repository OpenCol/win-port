/* ring_index_of -- shipped at 1018:411c.
 *
 * Ported by tools/port.py from matched/game/ring_index_of.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern signed char far g_ring_dx[];                  SEG27:0x0012
 *   extern signed char far g_ring_dy[];                  SEG27:0x0027
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 ring_index_of(col_i16 x, col_i16 y)
{
    col_i16 r = -1;
    col_i16 i;

    x -= 2;
    y -= 2;
    for (i = 0; i < 20; i++) {
        if (g_ring_dx[i] == (char)x && g_ring_dy[i] == (char)y) {
            r = i;
            break;
        }
    }
    return r;
}
