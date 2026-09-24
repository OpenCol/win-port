/* ring_tile_set_bits -- shipped at 1018:41df.
 *
 * Ported by tools/port.py from matched/game/ring_tile_set_bits.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern int g_stub_a;                                 
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
/* 0xca bytes */


extern void map_plane1_set_bits(col_i16 x, col_i16 y, col_i16 mask, col_i16 bits);



void ring_tile_set_bits(col_i16 x, col_i16 y, col_i16 bits)
{
    col_i16 i;

    i = ring_index_of(x, y);
    if (i >= 0) {
        x += g_cur_colony_rec->x - 2;
        y += g_cur_colony_rec->y - 2;
        map_plane1_set_bits(x, y, MAP1_CLAIM_SETTLED, bits);
    }
}
