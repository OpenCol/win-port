/* max_int_1038_86a6 -- shipped at 1038:86a6.
 *
 * Ported by tools/port.py from matched/game/max_int_1038_86a6.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int max_int_1038_86a6(int a, int b)
{
 if (a < b)
 return b;
 else
 return a;
}
