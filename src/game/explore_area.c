/* explore_area -- shipped at 1020:8b06.
 *
 * Ported by tools/port.py from matched/game/explore_area.c.
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
void explore_around_square(col_i16 x, col_i16 y, col_i16 nation, col_i16 radius, col_i16 is_ship);

#include "units.h"
/* 0x1c bytes at SEG20:0xa208 */


/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR; the body is
 * matched in matched/explore_around_square.c. */


void explore_area(col_i16 u, col_i16 radius)
{
    explore_around_square(g_units[u].x, g_units[u].y,
                                    (unsigned char)(g_units[u].flags & UNIT_NATION_MASK), radius,
                                    g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST);
}
