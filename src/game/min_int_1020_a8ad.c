/* min_int_1020_a8ad -- shipped at 1020:a8ad.
 *
 * Ported by tools/port.py from matched/game/min_int_1020_a8ad.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int min_int_1020_a8ad(int a, int b)
{
 if (a > b)
 return b;
 else
 return a;
}
