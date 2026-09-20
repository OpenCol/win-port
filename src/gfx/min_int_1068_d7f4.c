/* min_int_1068_d7f4 -- shipped at 1068:d7f4.
 *
 * Ported by tools/port.py from matched/gfx/min_int_1068_d7f4.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int min_int_1068_d7f4(int a, int b)
{
 if (a > b)
 return b;
 else
 return a;
}
