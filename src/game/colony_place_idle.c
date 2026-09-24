/* colony_place_idle -- shipped at 1018:7a42.
 *
 * Ported by tools/port.py from matched/game/colony_place_idle.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern int g_stub_a, g_stub_b;                       
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 best_colonist_job(col_i16 c, col_i16 slot);
col_i16 colonist_job(col_i16 c);
void colony_set_job(col_i16 c, col_i16 job);

void colony_place_idle(void)
{
    register col_i16 j;
    char i;
    char done[32];

    for (j = 0; j < g_cur_colony_rec->pop; j++)
        done[j] = 0;
    for (i = 0; (col_i16)i < 20; i++) {
        if (((signed char *)((unsigned char *)((col_i16)g_cur_colony_rec) + 0x70))[i] >= 0)
            done[((signed char *)((unsigned char *)(g_cur_colony_rec) + 0x70))[i]] = 1;
    }
    for (j = 0; j < g_cur_colony_rec->pop; j++) {
        if (done[j] == 0) {
            if (colonist_job(j) < 9) {
                if (best_colonist_job(j, -1) == 0)
                    done[j] = 1;
                else
                    colony_set_job(j, 13);
            }
        }
    }
}
