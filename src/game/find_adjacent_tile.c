/* find_adjacent_tile -- shipped at 1020:b624.
 *
 * Ported by tools/port.py from matched/game/find_adjacent_tile.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 ocean(col_i16 x, col_i16 y);
extern unsigned char square_region(col_i16 x, col_i16 y);

col_i16 find_adjacent_tile(col_i16 x0, col_i16 y0, col_i16 *px, col_i16 *py, col_i16 want)
{
    col_i16 x;
    register col_i16 y;
    col_i16 found;

    found = 0;
    for (x = x0; !found && x <= x0 + 1; x++) {
        for (y = y0; !found && y <= y0 + 1; y++) {
            if (ocean(x, y) == want) {
                if (want != 0 && square_region(x, y) != 1)
                    continue;
                *px = x;
                *py = y;
                found = 1;
            }
        }
    }
    return found;
}
