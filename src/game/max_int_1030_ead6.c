/* max_int_1030_ead6 -- shipped at 1030:ead6.
 *
 * Ported by tools/port.py from matched/game/max_int_1030_ead6.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int max_int_1030_ead6(int a, int b)
{
 if (a < b)
 return b;
 else
 return a;
}
