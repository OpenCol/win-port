/* octile_distance -- shipped at 1040:7b48.
 *
 * Ported by tools/port.py from matched/game/octile_distance.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int octile_distance(int a, int b)
{
 int r;

 a = a > 0 ? a : ~a + 1;
 b = b > 0 ? b : ~b + 1;
 if (a > b)
 r = (b >> 1) + a;
 else
 r = (a >> 1) + b;
 return r;
}
