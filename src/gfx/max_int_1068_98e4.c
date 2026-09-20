/* max_int_1068_98e4 -- shipped at 1068:98e4.
 *
 * Ported by tools/port.py from matched/gfx/max_int_1068_98e4.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int max_int_1068_98e4(int a, int b)
{
 if (a < b)
 return b;
 else
 return a;
}
