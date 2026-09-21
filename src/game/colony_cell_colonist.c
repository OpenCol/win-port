/* colony_cell_colonist -- shipped at 1018:423b.
 *
 * Ported by tools/port.py from matched/game/colony_cell_colonist.c.
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

/* 0xca bytes */





char colony_cell_colonist(col_i16 x, col_i16 y)
{
    char r = -1;
    register col_i16 i;

    i = ring_index_of(x, y);
    if (i >= 0)
        r = ((signed char *)((unsigned char *)(g_cur_colony_rec) + 0x70))[i];
    return r;
}
