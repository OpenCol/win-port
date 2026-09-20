/* min_int_1018_b329 -- shipped at 1018:b329.
 *
 * Ported by tools/port.py from matched/game/min_int_1018_b329.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int min_int_1018_b329(int a, int b)
{
 if (a > b)
 return b;
 else
 return a;
}
