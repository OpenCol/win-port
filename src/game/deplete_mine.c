/* deplete_mine -- shipped at 1018:903e.
 *
 * Ported by tools/port.py from matched/game/deplete_mine.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern char g_msg_seen;                              DGROUP:0x4bab
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char colony_cell_colonist(col_i16 a, col_i16 b);
char colony_event_message(char *msg, col_i16 a, col_i16 b, col_i16 c, col_i16 d, col_i16 e, col_i16 f);
col_i16 colonist_job(col_i16 c);

#include "terrain.h"
#include "jobs.h"
extern col_i16 tile_decoration(col_i16 x, col_i16 y);
extern void map_plane1_set_bits(col_i16 x, col_i16 y, col_i16 bits, col_i16 v);





void deplete_mine(void)
{
    col_i16 x;
    col_i16 colonist;
    col_i16 row;
    col_i16 y;
    col_i16 deco;
    col_i16 job;
    register col_i16 col;

    for (row = 0; row < 5; row++) {
        for (col = 0; col <= 5; col++) {
            colonist = colony_cell_colonist(col, row);
            if (colonist >= 0) {
                job = colonist_job(colonist);
                if (job == JOB_SILVER_MINER || job == JOB_ORE_MINER) {
                    x = g_cur_colony_rec->x + col - 2;
                    y = g_cur_colony_rec->y + row - 2;
                    deco = tile_decoration(x, y);
                    if (deco == 0xc || deco == 6) {
                        map_plane1_set_bits(x, y, MAP1_DEPLETED, 1);
                        g_msg_seen |= colony_event_message("DEPLETION", !g_msg_seen, -1, x, y, 3, 0);
                    }
                }
            }
        }
    }
}
