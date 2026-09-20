/* min_int_1060_641a -- shipped at 1060:641a.
 *
 * Ported by tools/port.py from matched/game/min_int_1060_641a.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int min_int_1060_641a(int a, int b)
{
 if (a > b)
 return b;
 else
 return a;
}
