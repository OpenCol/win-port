/* pair_score_table -- shipped at 1018:64ba.
 *
 * Ported by tools/port.py from matched/game/pair_score_table.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 pair_score_table(col_i16 a, col_i16 b)
{
    col_i16 x = a;
    col_i16 y = b;
    col_i16 r = 0;

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
