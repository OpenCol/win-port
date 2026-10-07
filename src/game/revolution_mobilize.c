/* revolution_mobilize: candidate for 1038:723e (531 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * At the declaration every colony of the nation with at least 50% rebel sentiment (`colony_rebel`) turns some of
 * its soldiers into continental soldiers: pop * (rebel% - 50) / 50, at most
 * half the population and at least one. Soldiers (type 1) become Continental
 * Army (9), dragoons (4) Continental Cavalry (7); only units on the colony's
 * chain with specialty 0x15 (soldier) qualify. Each colony that changed redraws its tile
 * and announces MOBILIZE (one man) or MOBILIZE2 (count in slot 0).
 *
 * RENAMED 2026-09-26 from mobilise_continental_army: the Mac build's revolution_mobilize(short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (callee colony_rebel; callee force_mood; callee next_unit; callee popm; callee popup_say), the runner-up gets 5, and the arity agrees (1).
 */

#include "colonization.h"

#include "jobs.h"
#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern void force_mood(int kind);
extern void set_colony(int colony);
extern int colony_rebel(void);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);

extern void popup_say(int slot, int word);
extern void force_map(int x, int y);
extern int popm(char *msg, int n);

void revolution_mobilize(char nation)
{
    int colony;
    int rebel_pct;
    int count;
    int n;
    int u;

    force_mood(3);
    for (colony = 0; g_game.colony_count > colony; colony++) {
        set_colony(colony);
        if ((char)g_cur_colony_rec->nation != nation)
            continue;
        rebel_pct = colony_rebel();
        if (rebel_pct < 50)
            continue;
        n = g_cur_colony_rec->pop * (rebel_pct - 50) / 50;
        n = min_int_1038_8680(n, g_cur_colony_rec->pop >> 1);
        n = max_int_1038_86a6(1, n);
        count = 0;
        for (u = top_unit_in_square(g_cur_colony_rec->x, g_cur_colony_rec->y); n > 0 && u >= 0; u = next_unit(u)) {
            if (g_units[u].type == UNIT_SOLDIERS || g_units[u].type == UNIT_DRAGOONS) {
                if ((int)g_units[u].spec == JOB_SOLDIER) {
                    n--;
                    set_name_slot_1038_8775(0, (char *)g_cur_colony_rec + 2);
                    popup_say(1, g_unit_types[g_units[u].type].name);
                    if (g_units[u].type == UNIT_SOLDIERS)
                        g_units[u].type = UNIT_CONTINENTAL_ARMY;
                    else
                        g_units[u].type = UNIT_CONTINENTAL_CAVALRY;
                    count++;
                }
            }
        }
        if (count != 0) {
            force_map(g_cur_colony_rec->x, g_cur_colony_rec->y);
            if (count == 1)
                popm("MOBILIZE", 1);
            else {
                set_text_arg_i_1038_86cc(0, count);
                popm("MOBILIZE2", 1);
            }
        }
    }
}
