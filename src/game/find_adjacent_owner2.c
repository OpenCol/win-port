/* find_adjacent_owner2 -- shipped at 1038:27d4.
 *
 * Ported by tools/port.py from matched/game/find_adjacent_owner2.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *   extern int g_foreign_owner;                          DGROUP:0x4c36
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "nations.h"



extern col_i16 ocean(col_i16 x, col_i16 y);
extern col_i16 square_nation(col_i16 x, col_i16 y);
extern col_i16 square_nation_bit0(col_i16 x, col_i16 y);

col_i16 find_adjacent_owner2(col_i16 x, col_i16 y, col_i16 nation)
{
    register col_i16 o;
    register col_i16 i;
    col_i16 nx;
    col_i16 ny;
    col_i16 oc;
    col_i16 t;
    col_i16 claimed;

    oc = ocean(x, y);
    claimed = square_nation(x, y) >= 0;
    g_foreign_owner = -1;
    for (i = 0; g_foreign_owner < 0 && i < 8; i++) {
        nx = g_dir_dx[i] + x;
        ny = g_dir_dy[i] + y;
        t = claimed != NATION_ENGLAND ? oc : ocean(nx, ny);
        o = square_nation(nx, ny);
        if (o < 0)
            o = square_nation_bit0(nx, ny);
        else
            oc = t;
        if (o >= 0 && o != nation && t == oc)
            g_foreign_owner = o;
    }
    return g_foreign_owner >= 0;
}
