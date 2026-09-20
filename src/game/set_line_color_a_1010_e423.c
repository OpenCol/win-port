/* set_line_color_a_1010_e423 -- shipped at 1010:e423.
 *
 * Ported by tools/port.py from matched/game/set_line_color_a_1010_e423.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_line_color_a;                      DGROUP:0x2add
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_line_color_a_1010_e423(char v)
{
 g_line_color_a = v;
}
