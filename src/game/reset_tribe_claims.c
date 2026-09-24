/* reset_tribe_claims -- shipped at 1030:a541.
 *
 * Ported by tools/port.py from matched/game/reset_tribe_claims.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Tribe far g_tribes[];                         SEG20:0xd182
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *   extern int far g_colony_count;                       SEG20:0x77a0
 *   extern RingRec far g_ring_counts_1[];                DGROUP:0x0c01
 *   extern signed char far g_ring_dx[];                  SEG27:0x0012
 *   extern signed char far g_ring_dy[];                  SEG27:0x0027
 *
 * The file's own Colony, Tribe, RingRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void tribe_joins_tory_side(col_i16 t);

/* 0xca bytes */
/* 0x4e bytes at SEG20:0xd182 */
extern void set_current_colony(col_i16 c);
extern col_i16 colony_ring_level(void);
extern char map_plane2_high_nibble(col_i16 x, col_i16 y);
extern col_i16 square_nation_any(col_i16 x, col_i16 y);
extern void map_plane2_set_high_nibble(col_i16 x, col_i16 y, col_i16 n);

/* same object: 4-byte `push cs; call ` (rule 47) */


void reset_tribe_claims(void)
{
    col_i16 c;
    col_i16 nation;
    col_i16 n;
    col_i16 cx;
    col_i16 cy;
    col_i16 x;
    col_i16 y;
    register col_i16 j;
    register col_i16 k;

    for (nation = 0; nation < 8; nation++)
        for (j = 0; j < 4; j++)
            ((col_i16 *)((unsigned char *)&(g_tribes[nation]) + 0x2e))[j] = 0;
    for (c = 0; c < 8; c++)
        if (!(g_tribes[c].flags & 0x80))
            tribe_joins_tory_side(c);
    for (c = 0; g_colony_count > c; c++) {
        set_current_colony(c);
        nation = g_cur_colony_rec->nation;
        cx = g_cur_colony_rec->x;
        cy = g_cur_colony_rec->y;
        n = (g_ring_counts_1 - 1)[colony_ring_level()].count;
        for (k = 0; k < n; k++) {
            x = g_ring_dx[k] + cx;
            y = g_ring_dy[k] + cy;
            if (((signed char *)((unsigned char *)((col_i16)g_cur_colony_rec) + 0x70))[k] < 0)
                continue;
            j = map_plane2_high_nibble(x, y);
            if (j < 4)
                continue;
            if (j == nation)
                continue;
            if (square_nation_any(x, y) >= 0)
                continue;
            map_plane2_set_high_nibble(x, y, nation);
        }
    }
}
