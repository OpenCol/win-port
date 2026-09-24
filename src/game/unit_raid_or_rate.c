/* unit_raid_or_rate -- shipped at 1030:9c93.
 *
 * Ported by tools/port.py from matched/game/unit_raid_or_rate.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 tribe_surprise_raid(col_i16 u);

extern void unit_move_dir(col_i16 u, col_i16 d);
extern void unit_refresh_move_points(col_i16 u);

/* same object: 4-byte `push cs; call ` */


void unit_raid_or_rate(col_i16 u)
{
    col_i16 d;

    d = tribe_surprise_raid(u);
    if (d != 8)
        unit_move_dir(u, d);
    else if (d >= 0)
        unit_refresh_move_points(u);
}
