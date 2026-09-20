/* max_int_1020_a887 -- shipped at 1020:a887.
 *
 * Ported by tools/port.py from matched/game/max_int_1020_a887.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int max_int_1020_a887(int a, int b)
{
 if (a < b)
 return b;
 else
 return a;
}
