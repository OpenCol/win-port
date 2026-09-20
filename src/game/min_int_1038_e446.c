/* min_int_1038_e446 -- shipped at 1038:e446.
 *
 * Ported by tools/port.py from matched/game/min_int_1038_e446.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int min_int_1038_e446(int a, int b)
{
 if (a > b)
 return b;
 else
 return a;
}
