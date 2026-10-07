/* CheatMenuHuman: candidate for 1048:bf8b (178 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The Cheat entry that hands a nation to the human. `SETHUMAN` asks which one, -1 is the
 * cancel, and the answer is one-based, so it is decremented before use.
 *
 * Every one of the four nations is marked 1 and then the chosen one is set back to 0, which
 * is what makes this function the evidence for what that byte means. Eleven matched files
 * called it `dead` and tested it `== 0` for "is the human player"; it is a THREE-state
 * control field -- 0 human, 1 computer, 2 defeated -- and §120 has the measurement.
 *
 * The whole body is one positive `if (n != -1) { ... }` and not an early `goto`: the shipped
 * test is a direct `JZ` over 118 bytes, where a `goto` gives Borland's invert-and-jump-over-
 * a-jump (rule 12). At this size the two are easy to tell apart and easy to write wrongly.
 *
 * RENAMED 2026-09-26 from set_human_player: the Mac build's CheatMenuHuman(void), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between game_menu_debug and MainWin_MainMenuProc, where the Mac gap holds 5 functions and the Windows one 3, so the pairing inside it is by size and arity (the Mac body is 1.21x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

#include "control.h"
#include "nations.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */
extern GameState g_game;   /* SEG20:0x7782 */

extern void DrawMap(int mode);

void CheatMenuHuman(void)
{
    int n;
    register int i;

    g_popup_dude = 3;
    n = ask_text_resource("SETHUMAN");
    if (n != -1) {
        n--;
        for (i = 0; i < NATION_EUROPEAN_COUNT; i++)
            g_nation_text[i].control = CONTROL_AI;
        if (n < NATION_EUROPEAN_COUNT) {
            g_nation_text[n].control = CONTROL_HUMAN;
            g_game.player_nation = n;
            g_game.turn_nation = n;
            g_game.view_nation = n;
            DrawMap(1);
        } else {
            if (ask_text_resource("SETAUTO") == 1)
                g_autoplay = 1;
        }
    }
    g_reselect_unit = 1;
}
