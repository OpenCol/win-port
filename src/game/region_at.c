/* region_at -- shipped at 1048:4235.
 *
 * Ported by tools/port.py from matched/game/region_at.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 hit_region_0(col_i16 x, col_i16 y);
col_i16 hit_region_1(col_i16 x, col_i16 y);
col_i16 hit_region_10(col_i16 x, col_i16 y);
col_i16 hit_region_2(col_i16 x, col_i16 y);
col_i16 hit_region_3(col_i16 x, col_i16 y);
col_i16 hit_region_4(col_i16 x, col_i16 y);

col_i16 region_at(col_i16 x, col_i16 y)
{
    col_i16 r = 15;
    if (hit_region_0(x, y))
        r = 0;
    else if (hit_region_2(x, y))
        r = 2;
    else if (hit_region_3(x, y))
        r = 3;
    else if (hit_region_1(x, y)) {
        r = 1;
        if (hit_region_10(x, y))
            r = 10;
    } else if (hit_region_4(x, y))
        r = 4;
    return r;
}
