/* pair_score_table -- shipped at 1018:64ba.
 *
 * Ported by tools/port.py from matched/game/pair_score_table.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int pair_score_table(int a, int b)
{
 int x = a;
 int y = b;
 int r = 0;

 if (x == 9 && y == 0) r += 2;
 if (x == 1 && y == 0) r += 2;
 if (x == 2 && y == 0) r += 2;
 if (x == 9 && y == 4) r += 2;
 if (x == 8 && y == 4) r += 3;
 if (x == 3 && y == 3) r = -1;
 if (x == 4 && y == 2) r = -1;
 if (x == 5 && y == 1) r = -1;
 if (x == 10 && y == 5) r += 2;
 if (x == 6 && y == 6) r += 3;
 if (x == 13 && y == 6) r += 2;
 if (x == 6 && y == 7) r += 1;
 if (x == 12 && y == 7) r += 2;
 if (x == 7 && y == 8) r += 3;
 return r;
}
