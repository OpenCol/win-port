/* building_is_useful -- shipped at 1018:9400.
 *
 * Ported by tools/port.py from matched/game/building_is_useful.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_tbl21_13e;                          SEG21:0x013e
 *   extern int far g_tbl21_140;                          SEG21:0x0140
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 current_colony_flag_get(col_i16 bit);

#include "building.h"






col_i16 building_is_useful(col_i16 b)
{
    col_i16 r = 0;

    if (b == 0)
        goto done;
    if (b == 5)
        goto done;
    if (b == 8)
        goto done;
    if (b == 14)
        goto done;
    if (b == 15)
        goto done;
    if (b == 6) {
        if (current_colony_flag_get(BUILDING_ARMORY) != 0)
            goto done;
        if (g_tbl21_13e != 0)
            goto done;
        if (g_tbl21_140 != 0)
            goto done;
    }
    r = 1;
done:
    return r;
}
