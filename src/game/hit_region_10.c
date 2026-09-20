/* hit_region_10 -- shipped at 1048:41fe.
 *
 * Ported by tools/port.py from matched/game/hit_region_10.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int hit_region_10(int x, int y)
{
 int inside = 0;
 if (x < 0x1b0 && x > 0x122 && y < 0x190 && y > 0x16e)
 inside = 1;
 return inside;
}
