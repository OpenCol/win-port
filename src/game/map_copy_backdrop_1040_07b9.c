/* map_copy_backdrop_1040_07b9 -- shipped at 1040:07b9.
 *
 * Ported by tools/port.py from matched/game/map_copy_backdrop_1040_07b9.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_map_window;                        SEG20:0x0658
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void map_copy_backdrop_1038_a573(void *mw);

void map_copy_backdrop_1040_07b9(void)
{
    map_copy_backdrop_1038_a573(&g_map_window);
}
