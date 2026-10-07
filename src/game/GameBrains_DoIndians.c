/* GameBrains_DoIndians: candidate for 1008:2b6d (121 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: GameBrains_DoIndians
 *
 * One step of the turn state machine, which ignores the context pointer its caller
 * passes. When nothing suppresses it: ranks the nations, clears byte +5 of every unit,
 * sets the viewed nation (SEG20:0x7798) to the player's (SEG20:0x779a), or to the override at
 * SEG20:0x77a6 when that is >= 0, and runs the tribes' turn (indian_turns).
 *
 * RENAMED 2026-09-26 from turn_step_units: the Mac build's GameBrains::DoIndians(void), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller TurnControlLoop; callee indian_turns), the runner-up gets 0, and the arity agrees (1). The Mac DoIndians has exactly two callees, indian_turns and set_ranks, and one caller, TurnControlLoop; this step ranks the nations and runs the tribes' turn.
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern void set_ranks(void);
extern void indian_turns(void);

void GameBrains_DoIndians(GameBrains *brains)
{
    int i;

    if (g_loaded_flag == 0) {
        set_ranks();
        for (i = 0; g_game.unit_count > i; i++)
            g_units[i].moves_used = 0;
        g_game.view_nation = g_game.player_nation;
        if (g_game.view_override >= 0)
            g_game.view_nation = g_game.view_override;
        indian_turns();
    }
}
