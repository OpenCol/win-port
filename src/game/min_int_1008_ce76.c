/* min_int_1008_ce76 -- shipped at 1008:ce76.
 *
 * Ported by tools/port.py from matched/game/min_int_1008_ce76.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int min_int_1008_ce76(int a, int b)
{
 if (a > b)
 return b;
 else
 return a;
}
