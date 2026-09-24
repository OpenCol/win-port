/* count_tile_units -- shipped at 1018:462f.
 *
 * Ported by tools/port.py from matched/game/count_tile_units.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern signed char far g_unit_type_class[];          SEG21:0x010a
 *   extern int g_tile_colonists;                         DGROUP:0x4bb2
 *   extern int g_tile_units;                             DGROUP:0x4bb4
 *   extern int g_tile_noncarriers;                       DGROUP:0x4bb6
 *   extern int g_tile_first_unit;                        DGROUP:0x4bb8
 *   extern int g_tile_sel;                               DGROUP:0x4bba
 *
 * The file's own Unit, UnitType, Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 min_int_1018_b329(col_i16 a, col_i16 b);
col_i16 unit_is_colonist(col_i16 u);

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */

/* 14 bytes at SEG20:0x7ac8 */

/* 0xca bytes */







extern col_i16 unit_at(col_i16 x, col_i16 y);
extern col_i16 unit_chain_next(col_i16 u);





void count_tile_units(void)
{
    register col_i16 u;

    g_tile_colonists = 0;
    g_tile_units = 0;
    g_tile_noncarriers = 0;
    g_tile_first_unit = unit_at(g_cur_colony_rec->x, g_cur_colony_rec->y);
    for (u = g_tile_first_unit; u >= 0; u = unit_chain_next(u)) {
        if (unit_is_colonist(u) != 0)
            g_tile_colonists++;
        if (g_unit_types[g_units[u].type].holds <= 0)
            g_tile_noncarriers++;
        g_tile_units++;
    }
    g_tile_colonists = min_int_1018_b329(50, g_tile_colonists);
    if (g_tile_noncarriers != 0 && g_tile_sel >= g_tile_noncarriers)
        g_tile_sel = 0;
}
