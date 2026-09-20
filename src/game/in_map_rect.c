/* in_map_rect -- shipped at 1038:b5c7.
 *
 * Ported by tools/port.py from matched/game/in_map_rect.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_view_x0;                                DGROUP:0x4c44
 *   extern int g_view_y0;                                DGROUP:0x4c46
 *   extern int g_view_x1;                                DGROUP:0x4c48
 *   extern int g_view_y1;                                DGROUP:0x4c4a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int in_map_rect(int x, int y)
{
 int r = 1;

 if (x < g_view_x0 || x > g_view_x1)
 r = 0;
 if (y < g_view_y0 || y > g_view_y1)
 r = 0;
 return r;
}
