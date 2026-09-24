/* best_move_dir -- shipped at 1028:0b21.
 *
 * Ported by tools/port.py from matched/game/best_move_dir.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern unsigned char far g_terrain_row_bytes[][0x10]; SEG20:0x78f0
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *
 * The file's own Unit, NationRec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 status_bits(col_i16 a, col_i16 t);

#include "nations.h"
#include "control.h"
#include "units.h"
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */





extern col_i16 square_nation_any(col_i16 x, col_i16 y);
extern col_i16 square_nation(col_i16 x, col_i16 y);
extern col_i16 unit_at(col_i16 x, col_i16 y);
extern col_i16 tile_unit_tally(col_i16 u, col_i16 kind);
extern col_i16 on_map(col_i16 x, col_i16 y);
extern col_i16 ocean(col_i16 x, col_i16 y);
extern col_i16 terrain_at(col_i16 x, col_i16 y);
extern col_i16 square_nation_bit0(col_i16 x, col_i16 y);
extern char map_plane2_high_nibble(col_i16 x, col_i16 y);
extern col_i16 nation_pair_flags_get(col_i16 a, col_i16 b);
extern unsigned char map_plane3_at(col_i16 x, col_i16 y);
extern col_i16 square_region_if_land(col_i16 x, col_i16 y);

/* same object: 4-byte `push cs; call ` */


col_i16 best_move_dir(col_i16 nation, col_i16 x, col_i16 y, col_i16 use_status, col_i16 soldier)
{
    col_i16 nx;
    col_i16 ny;
    col_i16 y2;
    col_i16 score;
    col_i16 best;
    col_i16 best_dir;
    col_i16 d;
    col_i16 d2;
    col_i16 ok;
    col_i16 unit;
    register col_i16 n;
    register col_i16 x2;

    best = -1;
    best_dir = 8;
    for (d = 0; d < 9; d++) {
        nx = g_dir_dx[d] + x;
        ny = g_dir_dy[d] + y;
        ok = 0;
        n = square_nation_any(nx, ny);
        if (n < 0) {
            ok = 1;
        } else if (square_nation(nx, ny) < 0 && n == nation) {
            unit = unit_at(nx, ny);
            if (tile_unit_tally(unit, 2) == 1
                && (g_units[unit].type == UNIT_ARTILLERY) != soldier)
                ok = 1;
        }
        if (on_map(nx, ny) != 0 && ocean(nx, ny) == 0 && (d == 8 || ok != 0)) {
            score = 0;
            score += g_terrain_row_bytes[terrain_at(nx, ny)][0xb];
            for (d2 = 0; d2 < 8; d2++) {
                x2 = g_dir_dx[d2] + nx;
                y2 = g_dir_dy[d2] + ny;
                if (on_map(x2, y2) != 0 && ocean(x2, y2) == 0) {
                    n = square_nation_bit0(x2, y2);
                    if (n < 0) {
                        n = map_plane2_high_nibble(x2, y2);
                        if ((n < 0 || n >= NATION_EUROPEAN_COUNT || g_nation52[n].control != CONTROL_HUMAN
                             || (nation_pair_flags_get(nation, n) & 0x40) == 0)
                            && use_status != 0)
                            score += (map_plane3_at(x2, y2) & 0xf)
                                   + (status_bits(nation, square_region_if_land(nx, ny)) << 4);
                    }
                }
            }
            if (score > best) {
                best = score;
                best_dir = d;
            }
        }
    }
    return best_dir;
}
