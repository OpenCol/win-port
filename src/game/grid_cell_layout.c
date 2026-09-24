/* grid_cell_layout -- shipped at 1048:4df1.
 *
 * Ported by tools/port.py from matched/game/grid_cell_layout.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void grid_cell_layout(col_i16 a, col_i16 b, col_i16 *band, col_i16 *pos,
               col_i16 *width, col_i16 *p6, col_i16 *p7)
{
    col_i16 t = 0;
    col_i16 v = a;
    col_i16 w;
    col_i16 cell;

    *band = 0;
    if (v < 3)
        t = v;
    else {
        (*band)++;
        v -= 3;
        if (v < 5)
            t = v;
        else
            (*band)++;
    }
    w = 0x20;
    cell = w + 1;
    *p6 = *p7 = w;
    *pos = t * cell + b;
    switch (*band) {
    case 0: *width = 0x138; break;
    case 1: *width = 0x16e; break;
    }
}
