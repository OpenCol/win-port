/* set_long_1ce4 -- shipped at 1040:605f.
 *
 * Ported by tools/port.py from matched/game/set_long_1ce4.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern long g_font_1ce4;                             DGROUP:0x1ce4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_long_1ce4(col_i32 v)
{
    g_font_1ce4 = v;
}
