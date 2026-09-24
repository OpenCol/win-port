/* copy_all_to_screen_1040_07d5 -- shipped at 1040:07d5.
 *
 * Ported by tools/port.py from matched/game/copy_all_to_screen_1040_07d5.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_map_window;                        SEG20:0x0658
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void copy_all_to_screen_1038_a6cf(void *mw);

void copy_all_to_screen_1040_07d5(void)
{
    copy_all_to_screen_1038_a6cf(&g_map_window);
}
