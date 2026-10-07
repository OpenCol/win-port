/* liberty_declared: candidate for 1038:7bc2 (178 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The Declare Independence command: refused after the revolution (ALREADYREVOLUTION), or
 * while rebel sentiment (SEG20:0x77d2) is under 50 (NONETOOTORY at 0, else TOOTORY after
 * set_text_arg_i_1038_86cc(0, sentiment)); otherwise the player's name slot is set and the DECLARE
 * dialog's answer 2 starts revolution.
 *
 * RENAMED 2026-09-26 from declare_independence: the Mac build's liberty_declared(void), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller MainMenuProc; callee popm; callee popup_country), the runner-up gets 1, and the arity agrees (0).
 */

#include "colonization.h"

#include "gameflag.h"
#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */
extern int popm(char *msg, int n);

extern void popup_country(int slot, int flag, int nation);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void revolution(void);
void liberty_declared(void)
{
    int r;

    if (g_game.game_flags & GAMEFLAG_INDEPENDENCE) {
        popm("ALREADYREVOLUTION", 1);
    } else if (g_game.rebel_sentiment < 0x32) {
        if (g_game.rebel_sentiment == 0) {
            popm("NONETOOTORY", 1);
        } else {
            set_text_arg_i_1038_86cc(0, g_game.rebel_sentiment);
            popm("TOOTORY", 1);
        }
    } else {
        popup_country(0, 0, g_game.player_nation);
        r = popm("DECLARE", 1);
        if (r != 2)
            goto done;
        revolution();
    }
done:
    ;
}
