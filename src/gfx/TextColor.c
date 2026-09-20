/* TextColor -- shipped at 1068:616a.
 *
 * Ported by tools/port.py from matched/gfx/TextColor.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_line_color_index;         DGROUP:0x2ada
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

unsigned char TextColor(unsigned char v)
{
    unsigned char old = g_line_color_index;
    g_line_color_index = v;
    return old;
}
