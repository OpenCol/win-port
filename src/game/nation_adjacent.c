/* nation_adjacent -- shipped at 1038:2a76.
 *
 * Ported by tools/port.py from matched/game/nation_adjacent.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 on_map(col_i16 x, col_i16 y);
extern col_i16 square_nation_bit0(col_i16 x, col_i16 y);
extern col_i16 square_european_nation(col_i16 x, col_i16 y);

col_i16 nation_adjacent(col_i16 x, col_i16 y, col_i16 nation)
{
    register col_i16 r = 0;
    register col_i16 i;
    col_i16 nx;
    col_i16 ny;

    if (on_map(x, y) == 0)
        goto done;
    for (i = 0; r == 0 && i < 8; i++) {
        nx = g_dir_dx[i] + x;
        ny = g_dir_dy[i] + y;
        r = square_nation_bit0(nx, ny) == nation;
        r |= square_european_nation(nx, ny) == nation;
    }
done:
    return r;
}
