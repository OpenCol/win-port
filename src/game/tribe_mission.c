/* tribe_mission: candidate for 1030:a6bc (710 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 710, not the census 702. The last 8 bytes are the switch's jump table.
 *
 * A MISSIONARY ESTABLISHES A MISSION in the current native settlement, and the tribe reacts.
 * The unit's record is freed, the settlement's mission byte (+5) takes the nation, and the
 * alarm the mission raises goes to `piss_off`.
 *
 * THE ALARM. It counts the nation's missions already in this tribe, then adjusts the count:
 *   - Juan de Sepulveda doubles it;
 *   - Bartolome de las Casas, Pocahontas and being France each halve it.
 * The tribe's mood toward the nation (`piss_factor` of `pissed`, 0..3) then sets the
 * base: `missions * 8` less 25, 15, 10 or 5, so a calm tribe can welcome a first mission.
 * Settlement flag 4 multiplies the swing by nine: `alarm += |alarm| * 8`. It is the same bit that
 * doubles the mission's own effect in `indian_village`.
 *
 * THE MESSAGE is MISSION0..MISSION3. It starts at the tribe's mood and is raised to at least 1
 * for alarm >= -5, at least 2 for alarm > 0, and to 3 for alarm >= 10. Its slots are:
 *   - 0: a word from the table at SEG20:0xd8b6;
 *   - 1: the nearest colony of the nation, or the nation's name if it has none;
 *   - 2: the season;
 *   - 3: `get_adjective(named)`.
 * The year goes into the first numeric argument. A human nation also hears sound 0x8024.
 *
 * The mission is an EXPERT one (bit 0x10) when the unit's specialty is JOB_MISSIONARY or the
 * nation has Jean de Brebeuf.
 *
 * Three spellings the bytes fixed:
 *   - the tribe filter is `continue`, not a nested `if`;
 *   - the absolute value is `~alarm + 1` (rule 11);
 *   - the specialty is compared through `(int)` (rule 51).
 *
 * RENAMED 2026-09-26 from establish_mission: the Mac build's tribe_mission(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 9 of its Mac call edges already bound land on this function (caller tribe_village; callee delete_unit; callee find_close_colony; callee has_father; callee pissed), the runner-up gets 7, and the arity agrees (3).
 */

#include "colonization.h"

#include "control.h"
#include "settle.h"
#include "nations.h"
#include "fathers.h"
#include "jobs.h"
#include "sounds.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_village(int s);
extern int has_father(int nation, int bit);
extern int pissed(int tribe, int i);
extern int piss_factor(int tension);

extern int find_close_colony(int x, int y, int nation, int land);
extern void popup_say(int slot, int word);
extern void sound_play(int n);
extern int popm(char *msg, int n);
extern void delete_unit(int u);
extern void piss_off(int tribe, int nation, int delta, int unused);
/* same object: 4-byte `push cs; call ` (rule 47) */

void tribe_mission(int u, int nation, int named)
{
    int mood;
    int col;
    int s;
    int tribe;
    int saved;
    char msg[0x28];
    register int missions;
    register int alarm;

    missions = 0;
    alarm = 0;
    tribe = g_cur_tribe;
    saved = g_cur_settlement;
    for (s = 0; s < g_game.settlement_count; s++) {
        set_village(s);
        if (g_cur_tribe != tribe)
            continue;
        if (((int)g_cur_settlement_rec->mission & MISSION_NATION_MASK) == nation)
            missions++;
    }
    set_village(saved);
    if (has_father(nation, FATHER_JUAN_DE_SEPULVEDA))
        missions += missions;
    if (has_father(nation, FATHER_BARTOLOME_DE_LAS_CASAS))
        missions >>= 1;
    if (has_father(nation, FATHER_POCAHONTAS))
        missions >>= 1;
    if (nation == NATION_FRANCE)
        missions >>= 1;
    mood = piss_factor(pissed(g_cur_tribe, nation));
    switch (mood) {
    case 0:
        alarm = (missions << 3) - 25;
        break;
    case 1:
        alarm = (missions << 3) - 15;
        break;
    case 2:
        alarm = (missions << 3) - 10;
        break;
    case 3:
    default:
        alarm = (missions << 3) - 5;
        break;
    }
    if (g_cur_settlement_rec->flags & SETTLEMENT_CAPITAL)
        alarm += (alarm > 0 ? alarm : ~alarm + 1) << 3;
    strcpy(msg, "MISSION0");
    if (alarm >= -5)
        mood = max_int_1030_ead6(1, mood);
    if (alarm > 0)
        mood = max_int_1030_ead6(2, mood);
    if (alarm >= 10)
        mood = 3;
    msg[7] += (char)mood;
    col = find_close_colony(g_cur_settlement_rec->x, g_cur_settlement_rec->y, nation, -1);
    popup_say(0, g_mission_word[nation]);
    if (col >= 0)
        set_name_slot(1, g_colonies[col].name);
    else
        popup_say(1, g_nation_name_word[nation]);
    popup_say(2, g_season_word[g_game.season]);
    g_text_args[0] = g_game.year;
    popup_say(3, get_adjective(named));
    if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
        sound_play(SOUND_AMEN);
        popm(msg, 4);
    }
    g_cur_settlement_rec->mission = nation;
    if ((int)g_units[u].spec == JOB_MISSIONARY || has_father(nation, FATHER_JEAN_DE_BREBEUF))
        g_cur_settlement_rec->mission |= MISSION_EXPERT;
    delete_unit(u);
    piss_off(g_cur_tribe, nation, alarm, 0);
}
