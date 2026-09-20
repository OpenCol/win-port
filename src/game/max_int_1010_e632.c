/* max_int_1010_e632 -- shipped at 1010:e632.
 *
 * Ported by tools/port.py from matched/game/max_int_1010_e632.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int max_int_1010_e632(int a, int b)
{
 if (a < b)
 return b;
 else
 return a;
}
