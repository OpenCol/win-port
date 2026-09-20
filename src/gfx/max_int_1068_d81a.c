/* max_int_1068_d81a -- shipped at 1068:d81a.
 *
 * Ported by tools/port.py from matched/gfx/max_int_1068_d81a.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int max_int_1068_d81a(int a, int b)
{
 if (a < b)
 return b;
 else
 return a;
}
