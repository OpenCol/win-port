/* min_int_1070_ddd8 -- shipped at 1070:ddd8.
 *
 * Ported by tools/port.py from matched/platform/min_int_1070_ddd8.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int min_int_1070_ddd8(int a, int b)
{
 if (a > b)
 return b;
 else
 return a;
}
