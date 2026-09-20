/* terrain_class -- shipped at 1038:c257.
 *
 * Ported by tools/port.py from matched/game/terrain_class.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_map_mode;                               DGROUP:0x1ca4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

char terrain_class(int v)
{
 v = (unsigned char)(v & 0x1f);
 switch (g_map_mode) {
 case 2:
 if (v < 0x18 && v >= 8)
 v = (v & 7) | 8;
 break;
 case 3:
 if (v < 0x18)
 v = v & 7;
 break;
 }
 return v;
}
