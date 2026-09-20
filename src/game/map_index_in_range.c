/* map_index_in_range -- shipped at 1038:b4ef.
 *
 * Ported by tools/port.py from matched/game/map_index_in_range.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_map_width;                              DGROUP:0x4c40
 *   extern int g_map_h;                                  DGROUP:0x4c42
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int map_index_in_range(int x, int y)
{
 if (x < 0 || y < 0 || (*g_map_width) - 1 < x || g_map_h - 1 < y)
 return 0;
 else
 return 1;
}
