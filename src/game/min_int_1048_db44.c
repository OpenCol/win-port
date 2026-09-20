/* min_int_1048_db44 -- shipped at 1048:db44.
 *
 * Ported by tools/port.py from matched/game/min_int_1048_db44.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int min_int_1048_db44(int a, int b)
{
 if (a > b)
 return b;
 else
 return a;
}
