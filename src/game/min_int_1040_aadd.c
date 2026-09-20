/* min_int_1040_aadd -- shipped at 1040:aadd.
 *
 * Ported by tools/port.py from matched/game/min_int_1040_aadd.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int min_int_1040_aadd(int a, int b)
{
 if (a > b)
 return b;
 else
 return a;
}
