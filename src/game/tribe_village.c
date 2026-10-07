/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 134 of the imported file. */
/* PORT: this file's own declarations of popi, tribe_incite removed: the header declares them from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 129 of the imported file. */
/* PORT: this file's own declarations of treaty_on, tribe_extortion, tribe_heresy, tribe_live removed: the header declares them from the definition (W1 step 3). */
/* tribe_village: candidate for 1030:db76 (2415 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: tribe_village
 * name-source: sibling-build
 * size-source: 2415, not the census 2373. The last 42 bytes are the two jump tables -- nine words
 * for the action switch and twelve for the unit-type switch.
 *
 * WHAT HAPPENS WHEN A UNIT ENTERS AN INDIAN VILLAGE. The whole encounter is here: the greeting,
 * the menu of things this particular unit can do, and the dispatch to whichever of nine handlers
 * the player picks.
 *
 * SHIPS ARE TURNED AWAY FIRST. A vessel (type 0x0d..0x12) at a village the tribe has not met
 * ("DONTKNOWSHIPS") or one the tribe is angry at -- attitude 0x4b or the mood word for this nation
 * at 0x40 -- gets a line and nothing else. The sound played on arrival is chosen from the tribe
 * class, 7 / 6 / 5, and is played only for a human-controlled European nation, and only while DGROUP:0xba is 0.
 *
 * A HUMAN-CONTROLLED EUROPEAN UNIT gets the long version: a woodcut, the tribe's level word and its
 * adjective dropped into name slots, and a greeting assembled by concatenation -- "VILLAGE" plus
 * one of "WAR", "BAD", "MEDIUM", "SAVAGE" or "HAPPY" chosen from the attitude, so the resource name
 * IS the sentence. The menu is then built item by item from what the unit is: trade (or, at attitude 0x4b, enter
 * the hostile village) for a wagon or a ship, "speak to the chief" for a scout, live among the natives for a
 * colonist the tribe can teach, and so on -- and the last item, cancel, is always added.
 *
 * A COMPUTER-CONTROLLED OR NATIVE UNIT SKIPS THE MENU ENTIRELY: the unit's type picks the action directly through a
 * twelve-way jump table. That is the asymmetry worth noticing -- the menu exists only for the human
 * player, and for everyone else a scout always scouts and a missionary always establishes a mission, with
 * one exception: type 3 (the missionary) runs a five-condition test -- the player's own standing
 * with the tribe, whether the tribe has met the player, the two nations' ranks, 1500 (0x5dc)
 * in the treasury and a `rand_range(0, 4)` that is not zero (four in five), unless the village already has a mission -- and on success offers to INCITE the tribe instead.
 *
 * `offered` exists to stop the "attack village" item being added twice: it is set when the
 * item goes on during the unit-type pass and tested before the same item would be added again.
 *
 * IF THE UNIT COUNT CHANGED while all this ran -- the village killed it, or gave the player a
 * new one -- the function returns 2 without refreshing anything. Otherwise 1 means the unit acted
 * and its rating is refreshed, 2 means it acted and did not, 0 means the unit attacks (treaty bit 4 is set); a cancel falls to the default and returns 1.
 *
 * WHAT THE BYTES SETTLED: the mission byte and the unit's specialty are SIGNED chars and every
 * comparison against them is widened with an explicit `(int)` -- without it Borland compares bytes
 * (rule 59's family). The unit-type switch's cases each leave through a `goto`, not a `break`,
 * because the shipped code jumps straight to the action switch where a break would hop twice. The
 * first-visit test is a structured if with NO else -- a long conditional (rule 12) and one explicit
 * `goto` out of the then-block, which is why there is no jump over the second half. And the action
 * switch carries an empty `default: break;`, visible only as the `EB 00` of rule 54.
 *
 * RENAMED 2026-09-26 from unit_enter_indian_village: the Mac build's tribe_village(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 9 of its Mac call edges already bound land on this function (caller move_unit; callee force_mood; callee get_name; callee get_treaty; callee item), the runner-up gets 7, and the arity agrees (3).
 */

#include "colonization.h"

#include "settle.h"
#include "vaction.h"
#include "woodcut.h"
#include "nations.h"
#include "tribes.h"
#include "jobs.h"
#include "units.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

extern Nation g_nations[];          /* SEG20:0xc2f4 */

extern GameState g_game;   /* SEG20:0x7782 */

extern int find_village(int x, int y);
extern void set_village(int s);
extern int pissed(int tribe, int nation);
extern void force_mood(int mood);
extern void popup_say(int slot, int word);

extern char *strings(int n);

extern int land_region_at(int x, int y);
extern int job_of_unit(int u);
extern void Popup_item(Popup *popup, char *text, int id);
extern void DrawMap(int flag);
extern void end_unit_turn(int u);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int get_treaty(int nation_a, int nation_b);

extern "C" int get_name(int nation);

extern "C" void tribe_trade(int unit, int settlement, int nation, int tribe_nation);

extern "C" void tribe_mission(int u, int nation, int tribe);
extern "C" int tribe_camp(int u, int nation, int tribe);

int tribe_village(register int unit, int x, int y)
{
    int result = 1;
    int village;
    int tribe_nation;
    int tribe_index;
    int choice;
    int attitude;
    int spare;
    int mood;
    int saved_units;
    int offered = 0;
    int nation;
    Popup pop(0x800);
    char msg[0x50];
    char line[0x50];

    saved_units = g_game.unit_count;
    village = find_village(x, y);
    set_village(village);
    nation = (unsigned char)(g_units[unit].flags & UNIT_NATION_MASK);
    tribe_nation = g_cur_settlement_rec->owner;
    tribe_index = tribe_nation - 4;
    attitude = pissed(tribe_index, nation);
    mood = g_cur_settlement_rec->anger[nation];
    if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == 0 && g_sound_music == 0) {
        if (g_cur_tribe == 0)
            force_mood(7);
        else if (g_cur_tribe == 1)
            force_mood(6);
        else
            force_mood(5);
    }
    if (g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_SHIP_LAST) {
        if ((get_treaty(nation, tribe_nation) & 0x20) == 0) {
            show_text_dialog_1030_ec0a((void*)("DONTKNOWSHIPS"));
            goto out;
        }
        if (attitude >= 0x4b || mood >= 0x40) {
            popup_say(0, get_adjective(tribe_nation));
            popi((void*)("MADATSHIPS"), g_cur_tribe);
            goto out;
        }
    }
    if (nation >= NATION_EUROPEAN_COUNT || g_nation_text[nation].control != 0) {
        switch (g_units[unit].type) {
        case UNIT_MISSIONARIES:
            if (pissed(g_cur_tribe, g_game.player_nation) < TRIBE_ATTITUDE_WAR
                && (get_treaty(g_game.player_nation, tribe_nation) & 0x20)
                && g_rank[nation] < g_rank[g_game.player_nation]
                && g_nations[nation].gold >= 0x5dcL
                && (rand_range(0, 4) != 0 || (int)g_cur_settlement_rec->mission >= 0)) {
                choice = VILLAGE_ACTION_INCITE;
                goto act;
            }
            if ((int)g_cur_settlement_rec->mission < 0) {
                choice = VILLAGE_ACTION_ESTABLISH_MISSION;
                goto act;
            }
            if ((g_cur_settlement_rec->mission & MISSION_NATION_MASK) != nation) {
                choice = VILLAGE_ACTION_DENOUNCE_HERESY;
                goto act;
            }
            break;
        case UNIT_WAGON_TRAIN:
            choice = VILLAGE_ACTION_TRADE;
            goto act;
        case UNIT_SOLDIERS:
        case UNIT_DRAGOONS:
        case UNIT_ARTILLERY:
            spare = land_region_at(g_units[unit].x, g_units[unit].y);
            choice = VILLAGE_ACTION_ATTACK;
            goto act;
        case UNIT_SCOUTS:
            choice = VILLAGE_ACTION_SPEAK_WITH_CHIEF;
            goto act;
        default:
            if (job_of_unit(unit) >= 0
                && ((int)g_units[unit].spec == JOB_COLONIST_ALT || (int)g_units[unit].spec == JOB_INDENTURED_SERVANT)
                && attitude < 0x4b) {
                choice = VILLAGE_ACTION_LIVE_AMONG_NATIVES;
                goto act;
            }
            break;
        }
        goto out;
    }
    {
        woodcut(WOODCUT_ENTERING_INDIAN_VILLAGE);
        popup_say(0, g_level[g_cur_tribe_rec->level].settlement);
        popup_say(1, get_name(tribe_nation));
        STRCPY(msg, "VILLAGE");
        if (attitude >= 0x4b)
            STRCAT(msg, "WAR");
        else if (attitude >= 0x32)
            STRCAT(msg, "BAD");
        else if (attitude >= 0x19 || g_cur_settlement_rec->anger[nation] >= SETTLEMENT_ANGER_HOSTILE)
            STRCAT(msg, "MEDIUM");
        else if (g_cur_tribe == 2)
            STRCAT(msg, "SAVAGE");
        else
            STRCAT(msg, "HAPPY");
        if (attitude >= 0x32) {
            if (g_cur_tribe == 0)
                force_mood(7);
            else if (g_cur_tribe == 1)
                force_mood(6);
            else
                force_mood(5);
        }
        show_plain_dialog_1030_ebb7(&pop, msg, 0);
        if (g_units[unit].type == UNIT_WAGON_TRAIN
            || (g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_SHIP_LAST)) {
            if (attitude < 0x4b)
                Popup_item(&pop, strings(g_action_word[VILLAGE_ACTION_TRADE - 1]), VILLAGE_ACTION_TRADE);
            else
                Popup_item(&pop, strings(g_action_word[VILLAGE_ACTION_ENTER_HOSTILE - 1]), VILLAGE_ACTION_ENTER_HOSTILE);
        }
        if (g_units[unit].type == UNIT_SCOUTS)
            Popup_item(&pop, strings(g_action_word[VILLAGE_ACTION_SPEAK_WITH_CHIEF - 1]), VILLAGE_ACTION_SPEAK_WITH_CHIEF);
        if (g_units[unit].type < UNIT_SHIP_FIRST || g_units[unit].type > UNIT_SHIP_LAST)
            if (g_unit_types[g_units[unit].type].attack > 1) {
                Popup_item(&pop, strings(g_action_word[VILLAGE_ACTION_ATTACK - 1]), VILLAGE_ACTION_ATTACK);
                offered = 1;
            }
        if (get_treaty(nation, tribe_nation) & 0x40) {
            if (g_units[unit].type == UNIT_MISSIONARIES) {
                if ((int)g_cur_settlement_rec->mission < 0)
                    Popup_item(&pop, strings(g_action_word[VILLAGE_ACTION_ESTABLISH_MISSION - 1]), VILLAGE_ACTION_ESTABLISH_MISSION);
                else if ((g_cur_settlement_rec->mission & MISSION_NATION_MASK) != nation) {
                    STRCPY(msg, strings(g_action_word[VILLAGE_ACTION_DENOUNCE_HERESY - 1]));
                    sprintf_rtl(line, msg,
                                strings(get_name(g_cur_settlement_rec->mission & MISSION_NATION_MASK)));
                    Popup_item(&pop, line, VILLAGE_ACTION_DENOUNCE_HERESY);
                }
                Popup_item(&pop, strings(g_action_word[VILLAGE_ACTION_INCITE - 1]), VILLAGE_ACTION_INCITE);
            } else {
                if (job_of_unit(unit) >= 0
                    && g_unit_types[g_units[unit].type].attack < 2
                    && g_units[unit].type != UNIT_SCOUTS
                    && job_of_unit(unit) != 0x1b)
                    Popup_item(&pop, strings(g_action_word[VILLAGE_ACTION_LIVE_AMONG_NATIVES - 1]), VILLAGE_ACTION_LIVE_AMONG_NATIVES);
                if (g_unit_types[g_units[unit].type].attack != 0
                    && (g_units[unit].type < UNIT_SHIP_FIRST || g_units[unit].type > UNIT_SHIP_LAST))
                    Popup_item(&pop, strings(g_action_word[VILLAGE_ACTION_DEMAND_TRIBUTE - 1]), VILLAGE_ACTION_DEMAND_TRIBUTE);
            }
        }
        if (offered == 0 && g_unit_types[g_units[unit].type].attack != 0
            && (g_units[unit].type < UNIT_SHIP_FIRST || g_units[unit].type > UNIT_SHIP_LAST))
            Popup_item(&pop, strings(g_action_word[VILLAGE_ACTION_ATTACK - 1]), VILLAGE_ACTION_ATTACK);
        Popup_item(&pop, strings(g_action_word[VILLAGE_ACTION_CANCEL - 1]), VILLAGE_ACTION_CANCEL);
        choice = popup_run_thunk_1030_ebe7(&pop) - 1;
    }
act:
    switch (choice) {
    case VILLAGE_ACTION_TRADE:
        tribe_trade(unit, village, nation, tribe_nation);
        break;
    case VILLAGE_ACTION_ENTER_HOSTILE:
        if (tribe_hostile(unit, village, nation, tribe_nation) != 0)
            result = 2;
        break;
    case VILLAGE_ACTION_ESTABLISH_MISSION:
        result = 2;
        tribe_mission(unit, nation, tribe_nation);
        break;
    case VILLAGE_ACTION_DENOUNCE_HERESY:
        tribe_heresy(unit, nation, tribe_nation);
        break;
    case VILLAGE_ACTION_INCITE:
        tribe_incite(unit, nation, tribe_nation);
        break;
    case VILLAGE_ACTION_SPEAK_WITH_CHIEF:
        if (tribe_camp(unit, nation, tribe_nation) != 0)
            result = 2;
        break;
    case VILLAGE_ACTION_LIVE_AMONG_NATIVES:
        tribe_live(unit, nation, tribe_nation, 0);
        break;
    case VILLAGE_ACTION_DEMAND_TRIBUTE:
        tribe_extortion(unit, nation, tribe_nation);
        break;
    case VILLAGE_ACTION_ATTACK:
        treaty_on(nation, tribe_nation, 4);
        result = 0;
        break;
    default:
        break;
    }
out:
    DrawMap(1);
    if (g_game.unit_count != saved_units)
        return 2;
    if (result == 1)
        end_unit_turn(unit);
    return result;
}

