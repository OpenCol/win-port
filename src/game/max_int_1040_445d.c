/* max_int_1040_445d -- shipped at 1040:445d.
 *
 * Ported by tools/port.py from matched/game/max_int_1040_445d.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int max_int_1040_445d(int a, int b)
{
 if (a < b)
 return b;
 else
 return a;
}
