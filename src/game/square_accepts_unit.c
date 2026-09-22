/* square_accepts_unit -- shipped at 1030:308a.
 *
 * Ported by tools/port.py from matched/game/square_accepts_unit.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "terrain.h"
extern col_i16 on_map(col_i16 x, col_i16 y);
extern col_i16 terrain_at(col_i16 x, col_i16 y);
extern col_i16 square_nation_bit0(col_i16 x, col_i16 y);

col_i16 square_accepts_unit(col_i16 u, col_i16 x, col_i16 y)
{
    col_i16 ok;
    col_i16 n;

    ok = 0;
    if (on_map(x, y) == 0)
        goto out;
    if (terrain_at(x, y) != TERRAIN_SEA_LANE)
        goto out;
    n = square_nation_bit0(x, y);
    if (n >= 0 && (unsigned char)(g_units[u].flags & 0xf) != n)
        goto out;
    ok = 1;
out:
    return ok;
}
