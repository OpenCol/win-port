/* terrain_from_map_byte -- shipped at 1040:42b5.
 *
 * Ported by tools/port.py from matched/game/terrain_from_map_byte.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "terrain.h"

int terrain_from_map_byte(int b)
{
 int t;

 if (b & MAP0_HILLY) {
 if (b & MAP0_MOUNTAINS)
 t = TERRAIN_MOUNTAINS;
 else
 t = TERRAIN_HILLS;
 } else {
 t = (unsigned char)(b & MAP0_TERRAIN_MASK);
 }
 return t;
}
