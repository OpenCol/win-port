/* colony_grid_metrics -- shipped at 1018:6222.
 *
 * Ported by tools/port.py from matched/game/colony_grid_metrics.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 min_int_1018_b329(col_i16 a, col_i16 b);

col_i16 colony_grid_metrics(col_i16 *pa, col_i16 *pb)
{
    register col_i16 n = 20;
    col_i16 r = n * 10;
    col_i16 t;

    if (pb != 0)
        *pb = n;
    if (pa != 0) {
        t = n * 12 + n - 1;
        *pa = min_int_1018_b329(t, 0x76);
    }
    return r;
}
