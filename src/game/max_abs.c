/* max_abs -- shipped at 1040:7bfc.
 *
 * Ported by tools/port.py from matched/game/max_abs.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int max_abs(int a, int b)
{
 int r;

 a = a > 0 ? a : ~a + 1;
 b = b > 0 ? b : ~b + 1;
 if (a > b)
 r = a;
 else
 r = b;
 return r;
}
