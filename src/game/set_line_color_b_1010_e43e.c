/* set_line_color_b_1010_e43e -- shipped at 1010:e43e.
 *
 * Ported by tools/port.py from matched/game/set_line_color_b_1010_e43e.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_line_color_b;                      DGROUP:0x2ade
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_line_color_b_1010_e43e(char v)
{
    g_line_color_b = v;
}
