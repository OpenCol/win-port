/* adjacent_foreign_owner -- shipped at 1038:28c9.
 *
 * Ported by tools/port.py from matched/game/adjacent_foreign_owner.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *   extern int g_foreign_owner;                          DGROUP:0x4c36
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 square_nation_bit0(col_i16 x, col_i16 y);

col_i16 adjacent_foreign_owner(col_i16 x, col_i16 y, col_i16 nation)
{
    register col_i16 i;
    register col_i16 owner;
    col_i16 nx;
    col_i16 ny;

    g_foreign_owner = -1;
    for (i = 0; g_foreign_owner < 0 && i < 8; i++) {
        nx = g_dir_dx[i] + x;
        ny = g_dir_dy[i] + y;
        owner = square_nation_bit0(nx, ny);
        if (owner >= 0 && owner != nation)
            g_foreign_owner = owner;
    }
    return g_foreign_owner >= 0;
}
