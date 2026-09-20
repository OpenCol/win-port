/* min_int_1050_91c4 -- shipped at 1050:91c4.
 *
 * Ported by tools/port.py from matched/game/min_int_1050_91c4.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int min_int_1050_91c4(int a, int b)
{
 if (a > b)
 return b;
 else
 return a;
}
