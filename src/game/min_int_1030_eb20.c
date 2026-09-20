/* min_int_1030_eb20 -- shipped at 1030:eb20.
 *
 * Ported by tools/port.py from matched/game/min_int_1030_eb20.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int min_int_1030_eb20(int a, int b)
{
 if (a > b)
 return b;
 else
 return a;
}
