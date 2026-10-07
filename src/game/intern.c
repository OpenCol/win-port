/* intern: candidate for 1038:484e (176 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: intern
 *
 * Removes a European nation: its relations with the two nations at
 * SEG20:0x779a/0x77d4 lose flag 0xb and gain 0x60, every unit of the nation
 * is freed (from the highest index down), and byte +1 of its 52-byte
 * record at SEG20:0x7840 becomes 2.  delete_unit is -called and
 * defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from nation_defeated: the Mac build's intern(short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (callee delete_unit; callee treaty_off; callee treaty_on), the runner-up gets 2, the arity agrees (1), and the Mac body is 0.99x this one's size. The margin is small, so the binding also rests on the body doing what the Mac name says.
 */

#include "colonization.h"

#include "control.h"
#include "units.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */
extern GameState g_game;   /* SEG20:0x7782 */
extern int g_stub_a;
extern int treaty_off(int nation, int other, int mask);
extern int treaty_on(int nation, int other, int flag);

extern void delete_unit(int u);

void intern(int nation)
{
    register int u;

    treaty_off(nation, g_game.player_nation, 0xb);
    treaty_off(nation, g_game.mother_country_idx, 0xb);
    treaty_on(nation, g_game.player_nation, 0x60);
    treaty_on(nation, g_game.mother_country_idx, 0x60);
    for (u = g_game.unit_count - 1; u >= 0; u--) {
        if ((char)(g_units[u].flags & UNIT_NATION_MASK) != (char)nation)
            continue;
        delete_unit(u);
    }
    g_nation_text[nation].control = CONTROL_DEFEATED;
}
