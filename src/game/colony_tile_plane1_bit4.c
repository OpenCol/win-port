/* colony_tile_plane1_bit4 -- shipped at 1018:416d.
 *
 * Ported by tools/port.py from matched/game/colony_tile_plane1_bit4.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 ring_index_of(col_i16 x, col_i16 y);

#include "terrain.h"
/* same object: 4-byte `push cs; call ` (rule 47) */

extern unsigned char map_plane1_at(col_i16 x, col_i16 y);

col_i16 colony_tile_plane1_bit4(col_i16 x, col_i16 y)
{
    col_i16 r = 0;
    col_i16 k;

    k = ring_index_of(x, y);
    if (k < 0)
        goto done;
    x += g_cur_colony_rec->x - 2;
    y += g_cur_colony_rec->y - 2;
    r = map_plane1_at(x, y) & MAP1_CLAIM_SETTLED;
done:
    return r;
}
