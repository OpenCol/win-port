/* unit_combat_strength -- shipped at 1020:7e61.
 *
 * Ported by tools/port.py from matched/game/unit_combat_strength.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern int g_combat_flags[2];                        DGROUP:0x4bda
 *   extern int g_combat_str[2];                          DGROUP:0x4be2
 *
 * The file's own Unit, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "fathers.h"
#include "jobs.h"
#include "units.h"
extern col_i16 nation_flag_test(col_i16 nation, col_i16 bit);
#define NATION(u) ((unsigned char)((u).flags & 0xf))

col_i16 unit_combat_strength(col_i16 unit, col_i16 attacking)
{
    col_i16 n = unit;
    col_i16 side;
    col_i16 str;
    col_i16 r;

    side = attacking ? 0 : 1;
    g_combat_flags[side] |= attacking ? 1 : 0;
    str = attacking ? g_unit_types[g_units[n].type].attack : g_unit_types[g_units[n].type].defence;
    if (g_units[n].type == UNIT_ARTILLERY && (g_units[n].f4 & 0x80))
        str -= 2;
    g_combat_str[side] = str;
    r = str << 3;
    if ((g_units[n].type == UNIT_SOLDIERS || g_units[n].type == UNIT_DRAGOONS) && (col_i16)g_units[n].spec == JOB_SOLDIER) {
        r += r >> 1;
        g_combat_flags[side] |= 2;
    }
    if (g_units[n].type == UNIT_PRIVATEER && nation_flag_test(NATION(g_units[n]), FATHER_FRANCIS_DRAKE)) {
        r += r >> 1;
        g_combat_flags[side] |= 0x4000;
    }
    if (g_units[n].type >= UNIT_SHIP_FIRST && g_units[n].type <= UNIT_SHIP_LAST) {
        r -= g_units[n].cargo_count;
        if (g_units[n].cargo_count != 0)
            g_combat_flags[side] |= 4;
    }
    return r;
}
