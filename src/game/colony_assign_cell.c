/* colony_assign_cell -- shipped at 1010:4354.
 *
 * Ported by tools/port.py from matched/game/colony_assign_cell.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *far g_cur_colony_rec;             DGROUP:0x4b9e
 *   extern char far g_grid_marks[5][5];                  SEG21:0x014a
 *   extern int far g_cell_col;                           DGROUP:0x0c08
 *   extern int far g_cell_row;                           DGROUP:0x0c0a
 *   extern int far g_selected_colonist;                  DGROUP:0x4bbc
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void tutorial_colony_food(void);

#include "jobs.h"
extern char colony_cell_colonist(col_i16 col, col_i16 row);
extern void set_pending_pair(col_i16 col, col_i16 row);
extern col_i16 colonist_job(col_i16 c);
extern col_i16 ocean(col_i16 x, col_i16 y);
extern col_i16 colony_assign_job(col_i16 c, col_i16 job);
extern void clear_colonist_cell(col_i16 c);
extern void colony_set_cell(col_i16 col, col_i16 row, col_i16 c);
extern void clear_colonist_specialty(col_i16 n);

/* same object: 4-byte `push cs; call ` (rule 47) */


void colony_assign_cell(void)
{
    col_i16 tx;
    col_i16 ty;
    register col_i16 job;
    register col_i16 ok;

    if (g_grid_marks[g_cell_col][g_cell_row] == 0) {
        if ((col_i16)colony_cell_colonist(g_cell_col, g_cell_row) == -1) {
            set_pending_pair(g_cell_col, g_cell_row);
            job = colonist_job(g_selected_colonist);
            if (job >= JOB_INDOOR_FIRST || job == JOB_FARMER || job == JOB_FISHERMAN) {
                tx = g_cur_colony_rec->x + g_cell_col - 2;
                ty = g_cur_colony_rec->y + g_cell_row - 2;
                if (ocean(tx, ty) != 0)
                    ok = !colony_assign_job(g_selected_colonist, 8);
                else
                    ok = !colony_assign_job(g_selected_colonist, 0);
            } else {
                ok = !colony_assign_job(g_selected_colonist, job);
            }
            if (ok != 0) {
                clear_colonist_cell(g_selected_colonist);
                colony_set_cell(g_cell_col, g_cell_row, g_selected_colonist);
            }
        } else {
            if (colony_cell_colonist(g_cell_col, g_cell_row)
                    != (char)g_selected_colonist)
                g_selected_colonist = colony_cell_colonist(g_cell_col, g_cell_row);
            else
                clear_colonist_specialty(0);
        }
    }
    tutorial_colony_food();
}
