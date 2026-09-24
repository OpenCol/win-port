/* unit_sight_or_range -- shipped at 1030:758b.
 *
 * Ported by tools/port.py from matched/game/unit_sight_or_range.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1030_ead6(col_i16 a, col_i16 b);

#include "units.h"
extern unsigned char unit_move_points(col_i16 u);



col_i16 unit_sight_or_range(col_i16 u)
{
    col_i16 r = unit_move_points(u) + 3;

    if (g_units[u].type == UNIT_PRIVATEER)
        r = r * 2;
    if (g_units[u].type == UNIT_GALLEON)
        r = r + 3;
    r -= g_units[u].cargo_count * 4;
    r = max_int_1030_ead6(1, r);
    return r;
}
