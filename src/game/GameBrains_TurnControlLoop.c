/* PORT: this file's own declarations of GameBrains_DoViceroy removed: the header declares it from the definition (W1 step 3). */
/* GameBrains_TurnControlLoop: candidate for 1008:3461 (285 bytes).  Verdict: RELOCATABLE.
 * symbol: GameBrains_TurnControlLoop
 * size-source: 285, not the census 273 -- the last 12 bytes are the switch's six-entry
 * name-source: sibling-build
 *   jump table, which the census cuts off at the RETF.
 *
 * THE TURN IS A STATE MACHINE AND THIS IS ITS PUMP. The phase number lives in the caller's
 * record at +2 and the loop runs phases one after another until one of them sets +6, which is
 * how a phase says "I need the player". Six phases: the natives (`GameBrains_DoIndians`), then the four
 * nations in order, then `GameBrains_DoEndofTurn` -- which clears the pending-message flag and
 * puts the phase back to 0.
 *
 * The loop condition tests `g_game.game_in_progress` as well as the record's flag, so a phase that
 * ends the game stops the pump immediately rather than running the rest of the turn, and the
 * call after the loop is skipped for the same reason.
 *
 * Coming in with the game already over does not run a phase at all: it sets the quit flag at
 * DGROUP:0x4b04 and returns, which is the only place in this function that writes it.
 *
 * The `switch` has no `default`: the `JA` past the table lands on the loop test, which is
 * exactly where falling out of the switch goes.
 *
 * RENAMED 2026-09-26 from turn_phase_step: the Mac build's GameBrains::TurnControlLoop(void): this only -- the turn state machine's pump, which the main loop runs on g_game_brains. Every function of the GameBrains module (1008:2927..3cc8) is called on the one object g_game_brains (SEG20:0x40d6), and the width matches.
 */

#include "colonization.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void GameBrains_DoIndians(GameBrains *ctx);
extern void GameBrains_DoEndofTurn(GameBrains *c);
extern void GameBrains_DoHuman(GameBrains *brains);

void GameBrains_TurnControlLoop(GameBrains *brains)
{
    if (g_game.game_in_progress != 0) {
        while (brains->awaiting_human == 0 && g_game.game_in_progress != 0) {
            switch (brains->step) {
            case 0:
                GameBrains_DoIndians(brains);
                brains->step++;
                break;
            case 1:
                GameBrains_DoViceroy(brains, 0);
                brains->step++;
                break;
            case 2:
                GameBrains_DoViceroy(brains, 1);
                brains->step++;
                break;
            case 3:
                GameBrains_DoViceroy(brains, 2);
                brains->step++;
                break;
            case 4:
                GameBrains_DoViceroy(brains, 3);
                brains->step++;
                break;
            case 5:
                GameBrains_DoEndofTurn(brains);
                g_game.end_of_turn_pending = 0;
                brains->step = 0;
                break;
            }
        }
        if (g_game.game_in_progress != 0)
            GameBrains_DoHuman(brains);
    } else {
        g_quit_flag = 1;
    }
}
