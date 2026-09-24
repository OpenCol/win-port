/* reset_nation_colours -- shipped at 1038:4f6d.
 *
 * Ported by tools/port.py from matched/game/reset_nation_colours.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_nation_colour[];          DGROUP:0x009a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void reset_nation_colours(void)
{
    g_nation_colour[0] = 10;
    g_nation_colour[1] = 11;
    g_nation_colour[2] = 12;
    g_nation_colour[3] = 13;
}
