/* set_colony: candidate for 1018:394a (153 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Records colony c as current: index at DGROUP:0x4ba2, record pointer at 0x4b9e (0xca-byte
 * records at SEG20:0x7c28; index 0 when out of range). The byte at 0x4baa becomes 1 when
 * the colony's owner (+0x1a) is the player nation (SEG20:0x7798), is a nation (below 4)
 * whose name-table flag is clear, and the index was in range. The byte at 0xc22 is cleared.
 *
 * RENAMED 2026-09-26 from set_current_colony: the Mac build's set_colony(short), bound by call-graph agreement (tools/mac_callgraph.py): 24 of its Mac call edges already bound land on this function (caller ColonyMouseMap; caller ColonyMouseUnits; caller DoBuildingFizzle; caller DrawABuilding; caller DrawATree), the runner-up gets 9, and the arity agrees (1).
 */

#include "colonization.h"

#include "control.h"
#include "nations.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

void set_colony(int colony)
{
    int oob = 0;

    g_cur_colony_idx = colony;
    if (colony < 0 || g_game.colony_count <= colony) {
        colony = 0;
        oob = 1;
    }
    g_cur_colony_rec = &g_colonies[colony];
    g_cur_colony_is_players = (char)(g_cur_colony_rec->nation == ((unsigned char)g_game.view_nation))
        && g_cur_colony_rec->nation < NATION_EUROPEAN_COUNT
        && g_nation_text[g_cur_colony_rec->nation].control == CONTROL_HUMAN
        && oob == 0;
    g_grid_marks_done = 0;
}
