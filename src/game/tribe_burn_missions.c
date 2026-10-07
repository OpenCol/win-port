/* tribe_burn_missions: candidate for 1038:0162 (205 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Every settlement of tribe `tribe` (byte +2 == tribe + 4) whose mission nibble (+5, low
 * four bits) belongs to `nation` loses it (+5 := -1); if any did, the tribe adjective and
 * the nation's name are announced and, for a human European nation, INDIANBURN shown.
 *
 * RENAMED 2026-09-26 from burn_mission: the Mac build's tribe_burn_missions(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller indian_turn; callee get_name; callee popm; callee popup_say), the runner-up gets 2, and the arity agrees (2).
 */

#include "colonization.h"

#include "nations.h"
#include "control.h"
#include "settle.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */
extern int get_name(int nation);

extern void popup_say(int slot, int word);
extern int popm(char *msg, int n);

void tribe_burn_missions(int tribe, int nation)
{
    int burned = 0;
    int i;

    for (i = 0; g_game.settlement_count > i; i++) {
        if (g_settlements[i].owner == (unsigned char)(tribe + 4) && (g_settlements[i].mission & MISSION_NATION_MASK) == nation) {
            burned = 1;
            g_settlements[i].mission = 0xff;
        }
    }
    if (burned) {
        popup_say(0, get_name(tribe + 4));
        popup_say(1, get_adjective(nation));
        if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN)
            popm("INDIANBURN", 1);
    }
}
