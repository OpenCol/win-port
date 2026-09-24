/* explore_around_square -- shipped at 1020:89ab.
 *
 * Ported by tools/port.py from matched/game/explore_around_square.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern int g_hint6_shown;                            DGROUP:0x0dca
 *
 * The file's own Unit, NationRec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void map_plane3_set_nation_bit(col_i16 x, col_i16 y, col_i16 nation, col_i16 flag);

#include "control.h"
#include "nations.h"
#include "woodcut.h"
#include "terrain.h"
/* 0x1c bytes at SEG20:0xa208 */

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern col_i16 on_map(col_i16 x, col_i16 y);
extern col_i16 ocean(col_i16 x, col_i16 y);
extern char map_plane1_at(col_i16 x, col_i16 y);
extern col_i16 square_region_if_land(col_i16 x, col_i16 y);
extern void woodcut_show(col_i16 hint);


/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR, which says
 * it was compiled into this object. The body is matched in
 * matched/map_plane3_set_nation_bit.c. */


void explore_around_square(col_i16 x, col_i16 y, col_i16 nation, col_i16 radius, col_i16 is_ship)
{
    col_i16 dy;
    col_i16 ty;
    col_i16 terr;
    col_i16 outer;
    register col_i16 dx;
    register col_i16 tx;

    if (on_map(x, y) == 0)
        goto done;
    if (nation >= NATION_EUROPEAN_COUNT)
        goto done;
    terr = square_region_if_land(x, y);
    for (dy = -radius; dy <= radius; dy++) {
        for (dx = -radius; dx <= radius; dx++) {
            tx = x + dx;
            ty = y + dy;
            if (on_map(tx, ty) == 0)
                continue;
            outer = (dy > 0 ? dy : ~dy + 1) > 1 || (dx > 0 ? dx : ~dx + 1) > 1;
            if (outer) {
                if (ocean(tx, ty) != is_ship)
                    continue;
                if (is_ship == 0 && square_region_if_land(tx, ty) != terr)
                    continue;
            } else if (g_hint6_shown == 0 && nation < NATION_EUROPEAN_COUNT && g_nation52[nation].control == CONTROL_HUMAN
                       && ocean(tx, ty) && (map_plane1_at(tx, ty) & MAP1_BIT5)) {
                g_hint6_shown = 1;
                woodcut_show(WOODCUT_DISCOVERY_OF_THE_PACIFIC_OCEAN);
            }
            map_plane3_set_nation_bit(tx, ty, nation, outer);
        }
    }
done:
    ;
}
