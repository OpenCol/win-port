/* adjacent_enemy -- shipped at 1028:0df4.
 *
 * Ported by tools/port.py from matched/game/adjacent_enemy.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *   extern int g_adjacent_colony_nation;                 DGROUP:0x4c12
 *
 * The file's own Unit, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 kind_if_known(col_i16 nation, col_i16 kind, col_i16 flag, col_i16 unit);

#include "units.h"
/* 14 bytes at SEG20:0x7ac8 */





extern col_i16 ocean(col_i16 x, col_i16 y);
extern col_i16 square_nation_bit0(col_i16 x, col_i16 y);
extern col_i16 unit_at(col_i16 x, col_i16 y);
extern col_i16 unit_chain_next(col_i16 u);
extern col_i16 square_nation(col_i16 x, col_i16 y);

/* same object: 4-byte `push cs; call ` */


col_i16 adjacent_enemy(col_i16 x, col_i16 y, col_i16 nation, col_i16 flag)
{
    col_i16 nx;
    col_i16 ny;
    col_i16 sea;
    col_i16 d;
    col_i16 r;
    col_i16 armed;
    register col_i16 unit;
    register col_i16 owner;

    r = -1;
    sea = ocean(x, y);
    g_adjacent_colony_nation = -1;
    for (d = 0; r < 0 && d < 8; d++) {
        nx = g_dir_dx[d] + x;
        ny = g_dir_dy[d] + y;
        if (ocean(nx, ny) != sea)
            continue;
        owner = square_nation_bit0(nx, ny);
        if (owner >= 0 && owner != nation) {
            if (owner >= 4)
                unit = unit_at(nx, ny);
            else
                unit = -1;
            r = kind_if_known(nation, owner, flag, unit);
            if (r >= 0 && sea != 0) {
                armed = 0;
                for (unit = unit_at(nx, ny); unit >= 0; unit = unit_chain_next(unit))
                    if (g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_SHIP_LAST
                        && g_unit_types[g_units[unit].type].attack != 0)
                        armed = 1;
                if (armed == 0)
                    r = -1;
            }
        }
        owner = square_nation(nx, ny);
        if (owner >= 0 && owner != nation) {
            r = kind_if_known(nation, owner, flag, -1);
            if (r < 4 && g_adjacent_colony_nation < 0)
                g_adjacent_colony_nation = r;
        }
    }
    return r;
}
