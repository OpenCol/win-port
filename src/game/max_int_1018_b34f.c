/* max_int_1018_b34f -- shipped at 1018:b34f.
 *
 * Ported by tools/port.py from matched/game/max_int_1018_b34f.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int max_int_1018_b34f(int a, int b)
{
 if (a < b)
 return b;
 else
 return a;
}
