/* vaction.h -- what a unit may do at a native village.
 *
 * Source: the ACTIONS resource (TEXT 3), ten lines in menu order, which `load_data_tables` reads
 * into `g_action_word[]` at SEG20:0xd908. `unit_enter_indian_village` builds its menu from that
 * array and then dispatches on the choice.
 *
 * THE IDS ARE ONE-BASED. The menu items are added with ids 1..10 and the switch tests those, so
 * `VILLAGE_ACTION_TRADE` is 1 while the resource line is `g_action_word[0]`. The header numbers
 * the SWITCH, because that is what the constants are compared against.
 *
 * Every handler names its own line, which is nine independent confirmations of the order:
 * `native_village_trade` for "Trade With Village", `establish_mission` for "Establish Mission",
 * `missionary_denounce` for "Denounce Heresy", `learn_from_natives` for "Live Among The Natives",
 * `speak_with_chief` for "Ask to Speak With Chief", `incite_tribe` for "Incite Indians" and
 * `extort_from_tribe` for "Demand Tribute".
 *
 * A TRIBE ALREADY MET NEVER SEES THE MENU: the unit's type picks the action directly through a
 * twelve-way jump table, so these ids are reached two ways and only one of them is a choice.
 */
#ifndef VACTION_H
#define VACTION_H

#define VILLAGE_ACTION_TRADE                 1   /* "Trade With Village" */
#define VILLAGE_ACTION_ENTER_HOSTILE         2   /* "Enter Hostile Village" */
#define VILLAGE_ACTION_ESTABLISH_MISSION     3   /* "Establish Mission" */
#define VILLAGE_ACTION_DENOUNCE_HERESY       4   /* "Denounce Heresy of %Fs Mission" */
#define VILLAGE_ACTION_LIVE_AMONG_NATIVES    5   /* "Live Among The Natives" */
#define VILLAGE_ACTION_SPEAK_WITH_CHIEF      6   /* "Ask to Speak With Chief" */
#define VILLAGE_ACTION_INCITE                7   /* "Incite Indians" */
#define VILLAGE_ACTION_DEMAND_TRIBUTE        8   /* "Demand Tribute" */
#define VILLAGE_ACTION_ATTACK                9   /* "Attack Village" */
#define VILLAGE_ACTION_CANCEL               10   /* "Cancel Action" */

#endif
