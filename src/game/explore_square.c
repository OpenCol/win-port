/* explore_square -- shipped at 1020:8b87.
 *
 * Ported by tools/port.py from matched/game/explore_square.c.
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
void explore_area(col_i16 u, col_i16 radius);

#include "fathers.h"
#include "units.h"
/* 0x1c bytes at SEG20:0xa208 */

extern col_i16 nation_flag_test(col_i16 nation, col_i16 bit);

/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR; the body is
 * matched in matched/explore_area.c. */


void explore_square(col_i16 u)
{
    col_i16 radius;

    radius = 1;
    if (g_units[u].type == UNIT_GALLEON || g_units[u].type == UNIT_PRIVATEER || g_units[u].type == UNIT_FRIGATE)
        radius = 2;
    if (nation_flag_test((unsigned char)(g_units[u].flags & UNIT_NATION_MASK), FATHER_HERNANDO_DE_SOTO)
        && (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST))
        radius = 2;
    if (g_units[u].type == UNIT_SCOUTS)
        radius++;
    explore_area(u, radius);
}
