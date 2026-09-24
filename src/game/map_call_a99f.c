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

extern void draw_unit_marks(void *w, void *sprite, col_i16 x, col_i16 y, col_i16 flags);

void map_call_a99f(void *sprite, col_i16 x, col_i16 y, col_i16 flags)
{
    draw_unit_marks((&g_map_window), sprite, x, y, flags);
}
