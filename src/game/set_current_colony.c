/* set_current_colony -- shipped at 1018:394a.
 *
 * Ported by tools/port.py from matched/game/set_current_colony.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern Colony far g_colonies[];                      SEG20:0x7c28
 *   extern int far g_colony_count;                       SEG20:0x77a0
 *   extern unsigned char far g_view_nation;              SEG20:0x7798
 *   extern int g_cur_colony_idx;                         DGROUP:0x4ba2
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern char g_cur_colony_is_players;                 DGROUP:0x4baa
 *   extern char g_grid_marks_done;                       DGROUP:0x0c22
 *
 * The file's own NationRec52, Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "control.h"
#include "nations.h"
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

void set_current_colony(col_i16 c)
{
    col_i16 oob = 0;

    g_cur_colony_idx = c;
    if (c < 0 || g_colony_count <= c) {
        c = 0;
        oob = 1;
    }
    g_cur_colony_rec = &g_colonies[c];
    g_cur_colony_is_players = (char)(g_cur_colony_rec->nation == g_view_nation)
        && g_cur_colony_rec->nation < NATION_EUROPEAN_COUNT
        && g_nation52[g_cur_colony_rec->nation].control == CONTROL_HUMAN
        && oob == 0;
    g_grid_marks_done = 0;
}
