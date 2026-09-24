/* tile_unit_tally -- shipped at 1038:2c51.
 *
 * Ported by tools/port.py from matched/game/tile_unit_tally.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *
 * The file's own Unit, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1038_86a6(col_i16 a, col_i16 b);
col_i16 unit_chain_tail(col_i16 u);

#include "jobs.h"
#include "units.h"
/* same object: 4-byte `push cs; call ` (rule 47) */

extern col_i16 unit_chain_next(col_i16 u);
extern col_i16 ocean(col_i16 x, col_i16 y);
extern col_i16 unit_combat_strength(col_i16 u, col_i16 mode);

col_i16 tile_unit_tally(col_i16 unit, col_i16 kind)
{
    register col_i16 u = unit;
    register col_i16 n = 0;

    for (u = unit_chain_tail(u); u >= 0; u = unit_chain_next(u)) {
        switch (kind) {
        case 0:
            n += g_unit_types[g_units[u].type].hammers;
            break;
        case 2:
            n++;
            break;
        case 3:
            if (g_units[u].type == UNIT_PIONEERS)
                n++;
            break;
        case 4:
            if (g_units[u].type == UNIT_SOLDIERS || g_units[u].type == UNIT_DRAGOONS)
                n++;
            if (g_units[u].type == UNIT_REGULARS || g_units[u].type == UNIT_CONTINENTAL_CAVALRY ||
                g_units[u].type == UNIT_CAVALRY || g_units[u].type == UNIT_CONTINENTAL_ARMY)
                n++;
            break;
        case 6:
            if (g_units[u].type == UNIT_DRAGOONS || g_units[u].type == UNIT_SOLDIERS || (col_i16)g_units[u].spec == JOB_SOLDIER)
                n++;
            if (g_units[u].type == UNIT_REGULARS || g_units[u].type == UNIT_CONTINENTAL_CAVALRY ||
                g_units[u].type == UNIT_CAVALRY || g_units[u].type == UNIT_CONTINENTAL_ARMY)
                n++;
            break;
        case 5:
            if (g_units[u].type == UNIT_SCOUTS)
                n++;
            break;
        case 10:
            if (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST) {
                if (g_unit_types[g_units[u].type].attack > 1)
                    n++;
            }
            break;
        case 11:
            if (ocean(g_units[u].x, g_units[u].y) ==
                (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST))
                n += unit_combat_strength(u, 1);
            break;
        case 12:
            if (g_units[u].type == UNIT_ARTILLERY)
                n++;
            break;
        case 13:
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST)
                n += g_unit_types[g_units[u].type].holds;
            break;
        case 14:
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST) {
                if ((g_units[u].f4 & 0x80) == 0 || g_units[u].type == UNIT_ARTILLERY)
                    n = max_int_1038_86a6(n, g_unit_types[g_units[u].type].holds);
            }
            break;
        }
    }
    return n;
}
