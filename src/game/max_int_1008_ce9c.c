/* max_int_1008_ce9c -- shipped at 1008:ce9c.
 *
 * Ported by tools/port.py from matched/game/max_int_1008_ce9c.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int max_int_1008_ce9c(int a, int b)
{
 if (a < b)
 return b;
 else
 return a;
}
