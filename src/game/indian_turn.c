/* indian_turn: candidate for 1030:a0a5 (1180 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A NATIVE TRIBE'S WHOLE TURN, which `indian_turns` runs for each tribe. The name,
 * derived from its one TEXT resource (INDIANGRUDGE), covers only its first step; `tribe_turn`
 * would describe it. The symbol is kept because `tools/re_textnames.py` assigns it.
 *
 *   1. THE TORY SIDE. During the revolution (bit 0 of 0x7784), a tribe that has not yet chosen
 *      (flag 0x20) joins the Crown when its alarm toward the player is 25+ and a 1..400 roll is at
 *      most that alarm, or when flag 0x40 forces it, and then only if a roll of 0..(10 - 2 x
 *      difficulty) is 0. INDIANGRUDGE is announced; the alarm toward the player rises by 100 and
 *      toward the Crown falls by 100; the player's missions in the tribe are burned; muskets and
 *      +8 are capped at the settlement count, muskets x4 and horses = 25 x +8. The tribe is marked.
 *   2. Muskets are floored at 0, and every settlement of the tribe takes its turn
 *      (`indian_village`).
 *   3. The tribe's 16 goods drift words move toward 0 by the tribe's level + 1.
 *   4. The tribe is re-tallied (`collect_indian_stats`), and horses grow by +8 up to
 *      `2 x population + 50`.
 *   5. UNITS: every unit of the tribe gets its idle counter (+0x16) cleared. Then, until a full pass
 *      acts with nobody, each unit that can still act (`enemy_is_active`) either raids or rates
 *      (`indian_unit`). A unit that has tried more than 20 times has its turn ended (`end_unit_turn`) and its counter
 *      reset instead.
 * `check_music` is called between the phases so the music keeps playing through a long turn.
 *
 * DEBUG PRINTS. A local `debug` flag is set to 0 and tested before five `printf`s ("Tribe: %d",
 * "Village %d", "Stats", "Unit %d (%d, %d)", "Finished"). They are compiled in but can never run.
 *
 * It compiled RELOCATABLE on the first build.
 *
 * RENAMED 2026-09-26 from tribe_joins_tory_side: the Mac build's indian_turn(short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (callee check_music; callee end_unit_turn; callee get_name; callee indian_village; callee pissed), the runner-up gets 3, and the arity agrees (1). This function's own header already said tribe_turn would describe it better: the Tory-side step is only the first of a native tribe's whole turn.
 */

#include "colonization.h"

#include "gameflag.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

extern GameState g_game;   /* SEG20:0x7782 */
extern void random_activate(int seed);
extern void set_tribe(int tribe);
extern void bottom_box(int colour);

extern int pissed(int tribe, int i);

extern void popup_say(int slot, int word);
extern void piss_off(int tribe, int nation, int delta, int unused);
extern void tribe_burn_missions(int tribe, int nation);

extern void check_music(void);
extern void end_unit_turn(int u);
extern int enemy_is_active(int u);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern int get_name(int nation);

extern void indian_village(int s);
extern void collect_indian_stats(int tribe);

extern void indian_unit(int u);

void indian_turn(int tribe)
{
    int drift;
    int alarm;
    int joins;
    int debug;
    int ta;
    int tb;
    int tc;
    int td;
    register int i;
    register int acted;

    debug = 0;
    random_activate(g_random_seed);
    g_game.turn_nation = tribe + 4;
    set_tribe(tribe);
    bottom_box(g_tribe_colour[tribe]);
    if (debug)
        printf_rtl("Tribe: %d\n", tribe);
    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) && !(g_cur_tribe_rec->flags & 0x20)) {
        alarm = pissed(tribe, g_game.player_nation);
        joins = alarm >= 0x19 && rand_range(1, 400) <= alarm ? 1 : 0;
        if (g_cur_tribe_rec->flags & 0x40)
            joins = 1;
        if (joins && rand_range(0, 10 - g_game.difficulty * 2) == 0) {
            popup_say(0, get_adjective(g_cur_tribe_nation));
            popup_say(1, get_name(g_cur_tribe_nation));
            show_text_dialog_1030_ec0a("INDIANGRUDGE");
            piss_off(g_cur_tribe, g_game.player_nation, 100, 0);
            piss_off(g_cur_tribe, g_game.mother_country_idx, -100, 0);
            tribe_burn_missions(g_cur_tribe, g_game.player_nation);
            g_cur_tribe_rec->muskets = *min_char_ptr_1030_f50a((char *)&g_cur_tribe_rec->muskets,
                                                              (char *)&g_tribe_settlement_count[g_cur_tribe]);
            g_cur_tribe_rec->muskets = g_cur_tribe_rec->muskets << 2;
            g_cur_tribe_rec->horse_herds = *min_char_ptr_1030_f50a((char *)&g_cur_tribe_rec->horse_herds,
                                                         (char *)&g_tribe_settlement_count[g_cur_tribe]);
            g_cur_tribe_rec->horses = g_cur_tribe_rec->horse_herds * 25;
            g_cur_tribe_rec->flags |= 0x20;
        }
    }
    g_cur_tribe_rec->muskets = max_int_1030_ead6(0, g_cur_tribe_rec->muskets);
    for (i = 0; g_game.settlement_count > i; i++) {
        if (g_settlements[i].owner == (unsigned char)g_game.turn_nation) {
            if (debug)
                printf_rtl("Village %d\n", i);
            indian_village(i);
            check_music();
        }
    }
    for (i = 0; i < 16; i++) {
        drift = g_cur_tribe_rec->goods_drift[i];
        if (drift > 0)
            g_cur_tribe_rec->goods_drift[i] =
                *max_ptr_1030_f4a6((tb = 0, &tb), (ta = drift - (g_cur_tribe_rec->level + 1), &ta));
        else if (drift < 0)
            g_cur_tribe_rec->goods_drift[i] =
                *min_ptr_1030_f474((td = 0, &td), (tc = g_cur_tribe_rec->level + drift + 1, &tc));
    }
    if (debug)
        printf_rtl("Stats\n");
    collect_indian_stats(tribe);
    check_music();
    if (g_cur_tribe_rec->horse_herds != 0) {
        g_cur_tribe_rec->horses += g_cur_tribe_rec->horse_herds;
        g_cur_tribe_rec->horses = min_int_1030_eb20(g_cur_tribe_rec->horses, g_tribe_pop[tribe] * 2 + 50);
    }
    for (i = 0; g_game.unit_count > i; i++)
        if (NATION(g_units[i]) == (unsigned char)g_game.turn_nation)
            g_units[i].counter = 0;
    do {
        check_music();
        for (acted = 0, i = 0; acted == 0 && g_game.unit_count > i; i++) {
            while (enemy_is_active(i)) {
                g_units[i].counter++;
                if (g_units[i].counter > 20) {
                    end_unit_turn(i);
                    g_units[i].counter = 0;
                    continue;
                }
                if (debug)
                    printf_rtl("Unit %d (%d, %d)\n", i, g_units[i].x, g_units[i].y);
                indian_unit(i);
                acted = 1;
            }
        }
    } while (acted != 0);
    if (debug)
        printf_rtl("Finished\n");
    set_tribe(tribe);
}
