/* mobilise_continental_army -- shipped at 1038:723e.
 *
 * Ported by tools/port.py from matched/game/mobilise_continental_army.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern int far g_colony_count;                       SEG20:0x77a0
 *
 * The file's own Unit, Colony, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1038_86a6(col_i16 a, col_i16 b);
col_i16 min_int_1038_8680(col_i16 a, col_i16 b);
col_i16 unit_at(col_i16 x, col_i16 y);
void set_name_slot_1038_8775(col_i16 slot, char *name);
void set_text_arg_i_1038_86cc(col_i16 slot, col_i16 v);

#include "jobs.h"
#include "units.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */

/* 14 bytes at SEG20:0x7ac8 */


extern void music_set_mood(col_i16 kind);
extern void set_current_colony(col_i16 c);
extern col_i16 colony_sol_percent(void);


/* same object: 4-byte `push cs; call ` (rule 47) */

extern col_i16 unit_chain_next(col_i16 u);

extern void set_name_slot_from_text(col_i16 slot, col_i16 word);
extern void redraw_tile(col_i16 x, col_i16 y);
extern void text_message_show(char *msg, col_i16 n);

void mobilise_continental_army(char nation)
{
    col_i16 c;
    col_i16 pop;
    col_i16 count;
    col_i16 n;
    col_i16 u;

    music_set_mood(3);
    for (c = 0; g_colony_count > c; c++) {
        set_current_colony(c);
        if (g_cur_colony_rec->nation != nation)
            continue;
        pop = colony_sol_percent();
        if (pop < 50)
            continue;
        n = g_cur_colony_rec->pop * (pop - 50) / 50;
        n = min_int_1038_8680(n, g_cur_colony_rec->pop >> 1);
        n = max_int_1038_86a6(1, n);
        count = 0;
        for (u = unit_at(g_cur_colony_rec->x, g_cur_colony_rec->y); n > 0 && u >= 0; u = unit_chain_next(u)) {
            if (g_units[u].type == UNIT_SOLDIERS || g_units[u].type == UNIT_DRAGOONS) {
                if ((col_i16)g_units[u].spec == JOB_SOLDIER) {
                    n--;
                    set_name_slot_1038_8775(0, (char *)g_cur_colony_rec + 2);
                    set_name_slot_from_text(1, g_unit_types[g_units[u].type].name);
                    if (g_units[u].type == UNIT_SOLDIERS)
                        g_units[u].type = UNIT_CONTINENTAL_ARMY;
                    else
                        g_units[u].type = UNIT_CONTINENTAL_CAVALRY;
                    count++;
                }
            }
        }
        if (count != 0) {
            redraw_tile(g_cur_colony_rec->x, g_cur_colony_rec->y);
            if (count == 1)
                text_message_show("MOBILIZE", 1);
            else {
                set_text_arg_i_1038_86cc(0, count);
                text_message_show("MOBILIZE2", 1);
            }
        }
    }
}
