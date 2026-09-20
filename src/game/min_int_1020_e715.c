/* min_int_1020_e715 -- shipped at 1020:e715.
 *
 * Ported by tools/port.py from matched/game/min_int_1020_e715.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int min_int_1020_e715(int a, int b)
{
 if (a > b)
 return b;
 else
 return a;
}
