/* unit_colonist_index -- shipped at 1018:45c8.
 *
 * Ported by tools/port.py from matched/game/unit_colonist_index.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern int g_tile_first_unit;                        DGROUP:0x4bb8
 *   extern int g_stub_a;                                 
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 unit_is_colonist(col_i16 u);

/* 0xca bytes */



extern col_i16 unit_chain_next(col_i16 u);



col_i16 unit_colonist_index(col_i16 target)
{
    col_i16 r = -1;
    register col_i16 k = -1;
    register col_i16 u;

    for (u = g_tile_first_unit; u >= 0 && r < 0; u = unit_chain_next(u)) {
        if (unit_is_colonist(u) != 0) {
            k++;
            if (u == target)
                r = g_cur_colony_rec->pop + k;
        }
    }
    return r;
}
