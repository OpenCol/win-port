/* PORT: this file's own declarations of popi removed: the header declares it from the definition (W1 step 3). */
/* tribe_camp: candidate for 1030:cde2 (1224 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 1224, not the census 1215: a dead second epilogue and two bytes of padding before 1030:d2aa.
 *
 * A SCOUT SPEAKS WITH THE CHIEF of the current native settlement, one of the actions `1030:db76`
 * dispatches. Returns 1 when the scout was killed, else 0.
 *
 * DEATH COMES FIRST:
 *   - always when the tribe's alarm toward the nation is 75 or more;
 *   - otherwise a roll 0..100 is made (0..140 for a JOB_SCOUT), and alarm 25 or more kills when a
 *     quarter of the alarm reaches the roll;
 *   - and the ARAWAKS (tribe 2) kill once more in `(8 - difficulty) << scout`.
 * A kill is cancelled, and the chief is merely bored, when the nation has flag bit 6. By the rule
 * that nation flag bits are Founding Father indices (four fathers witnessed it), that bit is
 * Francisco de Coronado. That fits the game less well than the rule's other witnesses, so the
 * constant is left as 6.
 * A killed scout is freed (sound SOUND_ARROW, CHIEFKILL).
 *
 * A HUMAN SCOUT THAT LIVES first hears CHIEFHOWDY. It names the skill taught here
 * (`tribe_live`) and the tribe's three most wanted goods: the 16-word table in data
 * segment 25, with the settlement's two already-known wants (+8, +9) zeroed out. The settlement's
 * `tribe_trade` report also runs.
 *
 * THEN THE CHIEF ANSWERS, once per settlement (flag 8 of +3), and only when the roll beat the
 * alarm; otherwise the chief is BORED. One of three, at random:
 *   1. a scout that is not yet seasoned becomes JOB_SCOUT: CHIEFGUIDES and WELLSEASONED. A scout
 *      that already is falls through to 2;
 *   2. the chief tells of the lands around: `explore_area(u, 6)` reveals radius 6 (CHIEFAREA);
 *   3. a GIFT of gold: three rolls of 1..(10 - difficulty), times 4, times 1..6, times the tribe's
 *      level + 1, added to the nation's treasury (CHIEFGIFT).
 *
 * THREE SPELLINGS the bytes settled:
 *   - the nation is a `register` COPY of the parameter, initialised first. As a register parameter it
 *     took DI, and a plain local was outranked for promotion by the busier `u` (rule 73);
 *   - the settlement's signed want bytes are compared through `(int)` (rule 51);
 *   - the roll is stored before the `switch`, and `switch (k = ...)` compiles differently.
 *
 * RENAMED 2026-09-26 from speak_with_chief: the Mac build's tribe_camp(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (callee explore_area; callee has_father; callee pissed; callee popm; callee popup_say), the runner-up gets 5, and the arity agrees (3).
 */

#include "colonization.h"

#include "fathers.h"
#include "control.h"
#include "settle.h"
#include "nations.h"
#include "jobs.h"
#include "sounds.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern Nation g_nations[];              /* SEG20:0xc2f4 */

extern GameState g_game;   /* SEG20:0x7782 */
extern int pissed(int tribe, int i);

extern void sort_16x16(int n, int *payload, int *key);
extern void popup_say(int slot, int word);
extern void DrawMap(int full);
extern int popm(char *msg, int n);
extern void set_center(int x, int y, int flag);

extern int has_father(int nation, int bit);
extern void sound_play(int n);
extern void delete_unit(int u);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int tribe_live(int u, int nation, int named, int ask);
extern void tribe_trade(int unit, int settlement, int nation, int tribe_nation);

int tribe_camp(int u, int nation_arg, int named)
{
    register int nation = nation_arg;
    register int k;
    int killed;
    int scout;
    int gift;
    int alarm;
    int roll;
    int skill;
    int i;
    int idx[16];

    killed = 0;
    scout = (int)g_units[u].spec == JOB_SCOUT ? 1 : 0;
    alarm = pissed(g_cur_tribe, nation);
    if (alarm >= 0x4b)
        goto kill;
    k = scout * 40 + 100;
    roll = rand_range(0, k);
    if (alarm >= 0x19 && alarm >> 2 >= roll)
        goto kill;
    if (g_cur_tribe == NATION_ARAWAKS - NATION_INCAS) {
        k = (8 - g_game.difficulty) << scout;
        if (rand_range(0, k) == 0)
            goto kill;
    }
    if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
        skill = tribe_live(u, nation, named, 1);
        tribe_trade(u, g_cur_settlement, -1, named);
        for (i = 0; i < 16; i++)
            idx[i] = (unsigned char)i;
        if ((int)g_cur_settlement_rec->sold >= 0)
            g_native_buy[(int)g_cur_settlement_rec->sold] = 0;
        if ((int)g_cur_settlement_rec->bought >= 0)
            g_native_buy[(int)g_cur_settlement_rec->bought] = 0;
        sort_16x16(16, idx, g_native_buy);
        popup_say(0, g_spec_recs[skill].expert);
        popup_say(1, g_goods_name_word[idx[15]]);
        popup_say(2, g_goods_name_word[idx[14]]);
        popup_say(3, g_goods_name_word[idx[13]]);
        popi("CHIEFHOWDY", g_cur_tribe);
    }
    popup_say(0, get_adjective(named));
    if (roll <= alarm)
        goto bored;
    if (g_cur_settlement_rec->flags & SETTLEMENT_CHIEF_SPOKE)
        goto bored;
    g_cur_settlement_rec->flags |= SETTLEMENT_CHIEF_SPOKE;
    k = rand_range(1, 3);
    switch (k) {
    case 1:
        if (scout == 0) {
            popup_say(1, g_level[g_cur_tribe_rec->level].settlement);
            g_units[u].spec = JOB_SCOUT;
            if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
                popi("CHIEFGUIDES", g_cur_tribe);
                DrawMap(1);
                popm("WELLSEASONED", 3);
            }
            goto done;
        }
    case 2:
        if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
            set_center(g_units[u].x, g_units[u].y, 0);
            popi("CHIEFAREA", g_cur_tribe);
        }
        explore_area(u, 6);
        if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN)
            DrawMap(0);
        goto done;
    case 3:
        popup_say(1, get_adjective(nation));
        k = 10 - g_game.difficulty;
        gift = rand_range(1, k) + rand_range(1, k) + rand_range(1, k);
        gift = gift * 4 * rand_range(1, 6);
        gift = (g_cur_tribe_rec->level + 1) * gift;
        set_text_arg_i_1030_eb46(0, gift);
        if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN)
            popi("CHIEFGIFT", g_cur_tribe);
        g_nations[nation].gold += gift;
        goto done;
    default:
        goto bored;
    }
kill:
    if (has_father(nation, FATHER_FRANCISCO_DE_CORONADO))    /* by the father-bit rule FATHER_FRANCISCO_DE_CORONADO: see the header */
        goto bored;
    killed = 1;
    popup_say(0, get_adjective(named));
    if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
        sound_play(SOUND_ARROW);
        popi("CHIEFKILL", g_cur_tribe);
    }
    delete_unit(u);
    goto done;
bored:
    popup_say(1, get_adjective(nation));
    if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN)
        popi("CHIEFBORED", g_cur_tribe);
    goto done;
done:
    return killed;
}
