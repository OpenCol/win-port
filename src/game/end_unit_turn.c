/* PORT: this file's own declarations of panel_clear_4 removed: the header declares it from the definition (W1 step 3). */
/* end_unit_turn: candidate for 1038:3bf5 (69 bytes, 42 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Recomputes a unit's rating into its byte +5 and, when it is the selected unit, redraws
 * the unit panel.
 *
 * RENAMED 2026-09-26 from unit_refresh_move_points: the Mac build's end_unit_turn(short), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (caller combat_fight; caller foreign_turn; caller foreign_unit; caller indian_move; caller move_unit), the runner-up gets 5, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern GameBrains g_game_brains;   /* SEG20:0x40d6 */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern unsigned char move_rate(int u);

void end_unit_turn(int unit)
{
    int u = unit;

    g_units[u].moves_used = move_rate(u);
    if (g_game.selected_unit == u)
        panel_clear_4(&g_game_brains, u);
}
