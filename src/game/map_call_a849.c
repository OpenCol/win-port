/* map_call_a849 -- shipped at 1040:20d3.
 *
 * Ported by tools/port.py from matched/game/map_call_a849.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_map_window[];                      SEG20:0x0658
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void draw_marker(void *w, col_i16 a, col_i16 b, col_i16 c, col_i16 d);

void map_call_a849(col_i16 a, col_i16 b, col_i16 c, col_i16 d)
{
    draw_marker((&g_map_window), a, b, c, d);
}
