/* map_call_a99f -- shipped at 1040:20fb.
 *
 * Ported by tools/port.py from matched/game/map_call_a99f.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_map_window[];                      SEG20:0x0658
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void draw_unit_marks(void *w, col_i16 a, col_i16 b, col_i16 c, col_i16 d, col_i16 e);

void map_call_a99f(col_i16 a, col_i16 b, col_i16 c, col_i16 d, col_i16 e)
{
    draw_unit_marks((&g_map_window), a, b, c, d, e);
}
