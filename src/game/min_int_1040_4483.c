/* min_int_1040_4483 -- shipped at 1040:4483.
 *
 * Ported by tools/port.py from matched/game/min_int_1040_4483.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int min_int_1040_4483(int a, int b)
{
 if (a > b)
 return b;
 else
 return a;
}
