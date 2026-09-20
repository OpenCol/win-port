/* hit_region_0 -- shipped at 1048:40ee.
 *
 * Ported by tools/port.py from matched/game/hit_region_0.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int hit_region_0(int x, int y)
{
 int inside = 0;
 if (x < 0x262 && x > 0 && y < 0x1c9 && y > 0x198)
 inside = 1;
 return inside;
}
