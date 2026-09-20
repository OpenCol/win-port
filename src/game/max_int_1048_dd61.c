/* max_int_1048_dd61 -- shipped at 1048:dd61.
 *
 * Ported by tools/port.py from matched/game/max_int_1048_dd61.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int max_int_1048_dd61(int a, int b)
{
 if (a < b)
 return b;
 else
 return a;
}
