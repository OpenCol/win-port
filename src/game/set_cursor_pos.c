/* set_cursor_pos: candidate for 1038:af9a (58 bytes).  Verdict: RELOCATABLE.
 *
 * Stores (x, y) in DGROUP:0x4b30/0x4b32 when the position is on the map.
 */

#include "colonization.h"

void set_cursor_pos(int x, int y)
{
    if (on_map(x, y) != 0) {
        g_cursor_x = x;
        g_cursor_y = y;
    }
}
