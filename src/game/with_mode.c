/* with_mode -- shipped at 1040:12a3.
 *
 * Ported by tools/port.py from matched/game/with_mode.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_redraw_mode;                        DGROUP:0x1ca0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void redraw_map_view(col_i16 a);

void with_mode(col_i16 a, col_i16 mode)
{
    g_redraw_mode = mode;
    redraw_map_view(a);
    g_redraw_mode = 0;
}
