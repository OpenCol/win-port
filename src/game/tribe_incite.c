/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 127, 132, 136, 141 of the imported file. */
/* PORT: this file's own declarations of popi removed: the header declares it from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 112 of the imported file. */
/* tribe_incite: candidate for 1030:d700 (1113 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: tribe_incite
 * name-source: sibling-build
 *
 * A MISSIONARY INCITES THE CURRENT TRIBE AGAINST ANOTHER EUROPEAN POWER, one of the actions
 * `1030:db76` dispatches. `tribe_nation` is the tribe's own nation number (4..11).
 *
 * THE PRICE, in gold:
 *   - the base is `2 * (strength / 8 - settlements) + 2 * (tribe +7) + 2 * (tribe +8) +
 *     8 * settlements`, from the tribe tables at SEG20:0xdc00 and 0xdc10;
 *   - it is multiplied by the tribe's alarm toward the inciting nation plus 75, so an angry tribe
 *     costs more, and France pays two thirds;
 *   - every one of the nation's missions among this tribe takes 250 off, 1000 for an expert
 *     mission (bit 0x10), doubled at a settlement with flag 4;
 *   - a Jesuit (JOB_MISSIONARY) takes 1500 off and a flag-4 settlement 500;
 *   - it is never below 500.
 *
 * A HUMAN picks the target from INDIANWARPATH, a menu of the other powers except the Crown. During
 * the revolution the target is always the Crown. Then:
 *   - NOCONTACT if the tribe has not met the target (pair flag 0x20);
 *   - INDIANWARPATH2 states the price; any answer but 1 ends it;
 *   - UNFORTUNATE when the treasury cannot pay;
 *   - ALREADYSMITE when the tribe's alarm toward the target is already 75 or more.
 * A COMPUTER nation always targets the player, silently, under the same three conditions.
 * On success INDIANWARFARE is announced, the tribe's alarm toward the target rises by 100, and
 * the price is paid.
 *
 * The owner test is a `continue`, and the mission test that follows it is an enclosing `if`: the
 * shipped jumps over the second with one short branch.
 *
 * RENAMED 2026-09-26 from incite_tribe: the Mac build's tribe_incite(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 9 of its Mac call edges already bound land on this function (caller tribe_village; callee get_name; callee get_treaty; callee item; callee pissed), the runner-up gets 5, and the arity agrees (3).
 */

#include "colonization.h"

#include "control.h"
#include "settle.h"
#include "gameflag.h"
#include "tribes.h"
#include "nations.h"
#include "jobs.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern Nation g_nations[];              /* SEG20:0xc2f4 */

extern GameState g_game;   /* SEG20:0x7782 */
extern int g_popup_tribe;                      /* DGROUP:0x2518, declared */
extern int pissed(int tribe, int i);
extern void set_village(int s);
extern void popup_say(int slot, int word);
extern char *strings(int n);
extern void Popup_item(Popup *popup, char *s, int id);
extern int popm(char *msg, int n);
extern void piss_off(int tribe, int nation, int delta, int unused);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" int get_name(int nation);

extern "C" int get_treaty(int nation_a, int nation_b);

extern "C" void tribe_incite(int u, register int nation, int tribe_nation)
{
    int saved;
    int s;
    int answer;
    long cost;
    long per;
    register int target;
    Popup p(0x800);

    cost = ((g_tribe_strength[g_cur_tribe] >> 3) - g_tribe_settlement_count[g_cur_tribe]) * 2;
    cost += g_cur_tribe_rec->muskets * 2;
    cost += g_cur_tribe_rec->horse_herds * 2;
    cost += g_tribe_settlement_count[g_cur_tribe] << 3;
    cost = (pissed(g_cur_tribe, nation) + 0x4b) * cost;
    if (nation == NATION_FRANCE)
        cost = cost * 2 / 3;
    saved = g_cur_settlement;
    for (s = 0; g_game.settlement_count > s; s++) {
        set_village(s);
        if (g_cur_settlement_rec->owner != (unsigned char)tribe_nation)
            continue;
        if (((int)g_cur_settlement_rec->mission & MISSION_NATION_MASK) == nation) {
            if (g_cur_settlement_rec->mission & MISSION_EXPERT)
                per = 1000L;
            else
                per = 250L;
            if (g_cur_settlement_rec->flags & SETTLEMENT_CAPITAL)
                per = per << 1;
            cost -= per;
        }
    }
    set_village(saved);
    if ((int)g_units[u].spec == JOB_MISSIONARY)
        cost -= 1500L;
    if (g_cur_settlement_rec->flags & SETTLEMENT_CAPITAL)
        cost -= 500L;
    cost = max_long_1030_ec52(500L, cost);
    if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
        if (g_game.game_flags & GAMEFLAG_INDEPENDENCE) {
            target = g_game.mother_country_idx;
        } else {
            g_popup_tribe = g_cur_tribe;
            popup_say(0, get_adjective(tribe_nation));
            show_plain_dialog_1030_ebb7(&p, (void*)("INDIANWARPATH"), 0);
            for (target = 0; target < 4; target++) {
                if (target == nation)
                    continue;
                if (g_game.mother_country_idx == target)
                    continue;
                Popup_item(&p, strings(get_name(target)), target + 1);
            }
            target = popup_run_thunk_1030_ebe7(&p);
            target = max_int_1030_ead6(1, target);
            target -= 2;
        }
        if (!(get_treaty(tribe_nation, target) & 0x20)) {
            popup_say(0, get_name(target));
            popi((void*)("NOCONTACT"), g_cur_tribe);
            goto done;
        }
        popup_say(0, get_adjective(target));
        set_text_arg_l_1030_ec2f(0, cost);
        answer = popi((void*)("INDIANWARPATH2"), g_cur_tribe);
        if (answer != 1)
            goto done;
        if (g_nations[nation].gold < cost) {
            popi((void*)("UNFORTUNATE"), g_cur_tribe);
            goto done;
        }
        if (pissed(g_cur_tribe, target) >= TRIBE_ATTITUDE_WAR) {
            popup_say(0, get_name(target));
            popi((void*)("ALREADYSMITE"), g_cur_tribe);
            goto done;
        }
    } else {
        target = g_game.player_nation;
        if (!(get_treaty(tribe_nation, target) & 0x20))
            goto done;
        if (pissed(g_cur_tribe, target) >= TRIBE_ATTITUDE_WAR)
            goto done;
        if (g_nations[nation].gold < cost)
            goto done;
    }
    popup_say(0, get_adjective(tribe_nation));
    popup_say(1, get_adjective(nation));
    popup_say(2, get_name(tribe_nation));
    popup_say(3, get_adjective(target));
    popm("INDIANWARFARE", 1);
    piss_off(g_cur_tribe, target, 100, 0);
    g_nations[nation].gold -= cost;
done:
    ;
}
