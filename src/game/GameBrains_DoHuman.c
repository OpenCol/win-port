/* GameBrains_DoHuman: candidate for 1008:3145 (796 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The interactive half of the turn pump: `GameBrains_TurnControlLoop` runs the automatic phases and
 * then hands the context here, which advances the human player's turn by one state of the word at
 * +4 each time it is called.
 *
 *   100  a nation's turn begins: the viewing nation is taken from +8 (or from SEG20:0x77a6 when
 *        that is set), the view relaid out, `autosave` called when bit 2 of
 *        SEG20:0x7785 is set and DGROUP:0x8b is clear, the first unit selected, and
 *        `buy_mercs` called
 *   0    pick the next unit that needs orders and act on it, or wait for input (state 1) in view
 *        mode; with the turn over or the game ended, go to state 3
 *   1    idle: keep the music going and blink the cursor square about every 25 ticks, toggling the
 *        cursor flag at DGROUP:0x78 and the status at SEG20:0xde0; back to state 0 when
 *        DGROUP:0x4b02 is raised
 *   2    back to state 0 after draining pending messages
 *   3    end: the done flag at +6 cleared, DGROUP:0x8b cleared and a message marked pending
 *
 * The cases are laid out in memory as 100, 0, 1, 2, 3, which is the source's order (rule 36).
 * `!x` is compiled as NEG / SBB / INC.
 *
 * `g_reselect_unit` (DGROUP:0x4b02) is what sends the machine back to phase 0, and phase 0 is
 * what answers the question 42884a42 left open. Phase 0 re-selects the active unit
 * (`GameBrains_EndUnitTurn`), re-lays the info window out and runs `GameBrains_ProcessActiveUnit`, then
 * CLEARS the flag; phase 1, the idle-and-blink loop, tests it and returns to phase 0
 * instead of blinking. Its two writers are both moments the selection may have gone
 * stale: `CheatMenuHuman`, and `ColonyWin_Show` the instant its modal loop returns.
 *
 * RENAMED 2026-09-26 from player_turn_step: the Mac build's GameBrains::DoHuman(void): this only -- the interactive half of the turn pump, advancing the human player's turn. Every function of the GameBrains module (1008:2927..3cc8) is called on the one object g_game_brains (SEG20:0x40d6), and the width matches.
 */

#include "colonization.h"

#include "gameflag.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */
extern char g_mini_win;          /* SEG20:0xde0 */
typedef struct { char pad[1]; } StatWin;
extern StatWin g_stat_win;           /* SEG20:0x1566 */

extern void GameWin_Redraw(GameWin *rec);
extern void set_europe(int n);
extern void buy_mercs(void);
extern int unit_special_active(int u);
extern void check_music(void);
extern int on_screen(int x, int y);
extern void UpdateTerrainRegion(int x, int y, int w, int h);
extern void ShowMapRegion(int x, int y, int w, int h);
extern void MiniWin_HighlightActiveUnit(void *rec, char on);

/* 5-byte NOP form, so extern (rule 47) */
extern void GameBrains_ProcessActiveUnit(GameBrains *c);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void autosave(void);
extern int GameBrains_EndUnitTurn(GameBrains *rec, int force);

extern void GameBrains_CursorUpdate(GameBrains *c);
extern void MSFlushInput_1008_cefc(void);

void GameBrains_DoHuman(GameBrains *brains)
{
    switch (brains->phase) {
    case 100:
        g_game.view_nation = brains->nation;
        g_cycle_flag = 0;
        if (g_game.view_override >= 0)
            g_game.view_nation = g_game.view_override;
        GameWin_Redraw((GameWin *)&g_stat_win);
        if (g_loaded_flag == 0 && (HI_BYTE(g_game.game_flags) & GAMEFLAG_HI_AUTOSAVE))
            autosave();
        g_game.human_turn_active = 1;
        g_game.selected_unit = -1;
        g_player_took_control = 0;
        GameBrains_EndUnitTurn(brains, 0);
        brains->phase = 0;
        g_reselect_unit = 0;
        set_europe(g_game.view_nation);
        buy_mercs();
        break;
    case 0:
        if (g_game.human_turn_active != 0 && g_game.game_in_progress != 0) {
            brains->unit_held = 0;
            if (g_game.view_mode == 0 && unit_special_active(g_game.selected_unit) == 0)
                brains->unit_held = GameBrains_EndUnitTurn(brains, 0);
            if (g_game.human_turn_active != 0) {
                if (brains->unit_held == 0)
                    GameWin_Redraw((GameWin *)&g_stat_win);
                if (g_game.view_mode == 0)
                    GameBrains_ProcessActiveUnit(brains);
                else
                    brains->phase = 1;
            }
        } else {
            brains->phase = 3;
        }
        g_reselect_unit = 0;
        break;
    case 1:
        check_music();
        if (g_reselect_unit != 0) {
            brains->phase = 0;
        } else if (g_game.human_turn_active != 0) {
            if (g_game.view_mode == 0)
                g_player_took_control = 1;
            if ((long)MSGetTicks_1008_cee5() > (long)(g_last_blink + 25)) {
                g_last_blink = MSGetTicks_1008_cee5();
                g_cycle_flag = !g_cycle_flag;
                if (g_game.end_of_turn_pending != 0)
                    GameWin_Redraw((GameWin *)&g_stat_win);
                if (on_screen(g_cursor_x, g_cursor_y)) {
                    UpdateTerrainRegion(g_cursor_x, g_cursor_y, 1, 1);
                    GameBrains_CursorUpdate(brains);
                    MiniWin_HighlightActiveUnit((MiniWin *)&g_mini_win, g_blink = !g_blink);
                    ShowMapRegion(g_cursor_x, g_cursor_y, 1, 1);
                } else {
                    MiniWin_HighlightActiveUnit((MiniWin *)&g_mini_win, g_blink = !g_blink);
                }
            }
        } else {
            brains->phase = 3;
        }
        break;
    case 2:
        brains->phase = 0;
        MSFlushInput_1008_cefc();
        break;
    case 3:
        brains->awaiting_human = 0;
        g_loaded_flag = 0;
        g_game.end_of_turn_pending = 1;
        break;
    }
}
