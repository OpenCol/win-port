/* map_view_layout_1040_0597 -- shipped at 1040:0597.
 *
 * Ported by tools/port.py from matched/game/map_view_layout_1040_0597.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_map_window;                        SEG20:0x0658
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void map_view_layout(void *mw);

void map_view_layout_1040_0597(void)
{
    map_view_layout(&g_map_window);
}
