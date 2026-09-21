/* count_plane1_around -- shipped at 1018:646a.
 *
 * Ported by tools/port.py from matched/game/count_plane1_around.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 map_plane1_test_bits(col_i16 x, col_i16 y, col_i16 bits);

col_i16 count_plane1_around(col_i16 x, col_i16 y, col_i16 bits)
{
    register col_i16 n = 0;
    register col_i16 dir;

    for (dir = 0; dir < 8; dir++)
        n += map_plane1_test_bits(g_dir_dx[dir] + x, g_dir_dy[dir] + y, bits);
    return n;
}
