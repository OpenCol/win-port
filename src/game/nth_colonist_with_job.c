/* nth_colonist_with_job -- shipped at 1018:5a60.
 *
 * Ported by tools/port.py from matched/game/nth_colonist_with_job.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 0xca bytes */


col_i16 nth_colonist_with_job(unsigned char job, col_i16 nth)
{
    col_i16 r = -1;
    col_i16 k = -1;
    col_i16 c;

    for (c = 0; r < 0 && c < g_cur_colony_rec->pop; c++) {
        if (((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0x20))[c] == job) {
            k++;
            if (k == nth)
                r = c;
        }
    }
    return r;
}
