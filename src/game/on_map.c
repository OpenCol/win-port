/* on_map -- shipped at 1038:b4b6.
 *
 * Ported by tools/port.py from matched/game/on_map.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_map_width;                              DGROUP:0x4c40
 *   extern int g_map_h;                                  DGROUP:0x4c42
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 on_map(col_i16 x, col_i16 y)
{
    col_i16 r = 1;

    if (x < 1 || y < 1 || (*g_map_width) - 1 < x || g_map_h - 1 < y)
        r = 0;
    return r;
}
