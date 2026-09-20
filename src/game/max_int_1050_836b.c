/* max_int_1050_836b -- shipped at 1050:836b.
 *
 * Ported by tools/port.py from matched/game/max_int_1050_836b.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int max_int_1050_836b(int a, int b)
{
 if (a < b)
 return b;
 else
 return a;
}
