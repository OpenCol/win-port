/* hit_region_2 -- shipped at 1048:4123.
 *
 * Ported by tools/port.py from matched/game/hit_region_2.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int hit_region_2(int x, int y)
{
 int inside = 0;
 if (x < 0x11a && x > 0x8f && y < 0x17b && y > 0x102)
 inside = 1;
 return inside;
}
