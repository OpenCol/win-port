/* colony_owns_tile -- shipped at 1018:3f85.
 *
 * Ported by tools/port.py from matched/game/colony_owns_tile.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern RingRec g_ring_counts_1[];                    DGROUP:0x0c01
 *   extern signed char far g_ring_dx[];                  SEG27:0x0012
 *   extern signed char far g_ring_dy[];                  SEG27:0x0027
 *   extern int g_stub_a;                                 
 *
 * The file's own Colony, RingRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 colony_ring_level(void);

/* 0xca bytes */

extern col_i16 on_map(col_i16 x, col_i16 y);



col_i16 colony_owns_tile(col_i16 x, col_i16 y)
{
    register col_i16 found = 0;
    register col_i16 i;
    col_i16 n;

    if (on_map(x, y) == 0)
        goto done;
    n = (g_ring_counts_1 - 1)[colony_ring_level()].count;
    if (g_cur_colony_rec->x == (unsigned char)x && g_cur_colony_rec->y == (unsigned char)y)
        found = 1;
    for (i = 0; found == 0 && i < n; i++) {
        if ((unsigned char)(g_ring_dx[i] + g_cur_colony_rec->x) == (unsigned char)x
            && (unsigned char)(g_cur_colony_rec->y + g_ring_dy[i]) == (unsigned char)y)
            found = 1;
    }
done:
    return found;
}
