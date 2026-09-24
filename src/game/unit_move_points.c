/* unit_move_points -- shipped at 1038:21b1.
 *
 * Ported by tools/port.py from matched/game/unit_move_points.c.
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

#include "fathers.h"
#include "units.h"
extern col_i16 nation_flag_test(col_i16 nation, col_i16 bit);
#define NATION(u) ((unsigned char)((u).flags & 0xf))

unsigned char unit_move_points(col_i16 i)
{
    col_i16 n = i;
    unsigned char r;

    r = g_unit_types[g_units[n].type].moves;
    if (nation_flag_test(NATION(g_units[n]), FATHER_FERDINAND_MAGELLAN) && g_units[n].type >= UNIT_SHIP_FIRST && g_units[n].type <= UNIT_SHIP_LAST)
        r += 3;
    return r;
}
