/* colony_ring_level -- shipped at 1018:3f16.
 *
 * Ported by tools/port.py from matched/game/colony_ring_level.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 current_colony_building_count(col_i16 b);
col_i16 min_int_1018_b329(col_i16 a, col_i16 b);

#include "building.h"






col_i16 colony_ring_level(void)
{
    register col_i16 n;

    n = current_colony_building_count(BUILDING_TOWN_HALL_2);
    n = min_int_1018_b329(2, n);
    return n + 2;
}
