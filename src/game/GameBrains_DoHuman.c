/* PORT: this file's own declarations of MiniWin_HighlightActiveUnit removed: the header declares it from the definition (W1 step 3). */
/* PORT: water colour cycling, win-patches' water-cycling in C: water_cycle_tick before each cursor-blink call of MiniWin_HighlightActiveUnit (lines marked PORT: below). */
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

/* 5-byte NOP form, so extern (rule 47) */
extern void GameBrains_ProcessActiveUnit(GameBrains *c);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void autosave(void);
extern int GameBrains_EndUnitTurn(GameBrains *rec, int force);

extern void GameBrains_CursorUpdate(GameBrains *c);
extern void MSFlushInput_1008_cefc(void);

/* PORT: water colour cycling (colonization-re/win-patches, patches/water-cycling: the same
   tick, there written over the dead palette_cross_fade and hooked onto this function's two calls
   of MiniWin_HighlightActiveUnit). The DOS build rotates 8 palette entries from the timer, and
   only river centre lines, beach surf and sea sparkles use them; the Windows water sheet
   (picture 0xcc, installed at palette 40 with 102 colours) reserved entries 136..141 for the same
   pixels, and Game Options kept "Water Color Cycling" with its saved bit
   (GAMEFLAG_NO_WATER_CYCLING, set = off) -- but nothing ever rotated them. On each cursor blink
   (25 ticks, 0.42 s) while the player is on the map, unless the option is off: rotate them up by
   one through the game's own palette_rotate, as DOS does; and re-blit the map view. The new
   colours have to reach the screen, two ways:
   - on a palette display (g_palette_mode 1; the port's display is one, in software:
     src/host/w16_display.c) the six entries go into the logical palette (MSUpdatePalette) and the
     system palette. Not palette_rotate's AnimatePalette: the game's entries are PC_NOCOLLAPSE,
     not PC_RESERVED, so the logical palette would keep the old colours and the next
     RealizePalette put them back; and the port's AnimatePalette re-blits every window, which a
     session's fast virtual clock (some 77 blinks a script step) paid for three to five times its
     run. w16_palette_quiet sets the system entries and the map alone is re-blitted;
   - on a display without one (g_palette_mode 0, $PORT_TRUECOLOR), the patch's way: palette_rotate
     (its AnimatePalette does nothing in mode 0) and the palette's seed bumped, so GRPort_ColorSync
     re-sends the WinG colour table. Incremented, not re-rolled with CTable_Reseed: that draws
     from the C runtime's rand(), which the rules use.
   Then the map view is re-blitted.
   A change from the 1995 program, which never moved its water: Daniel's choice (2026-10-12), on
   by default as the option's default says. */
extern void w16_palette_quiet(unsigned start, unsigned n, const void *entries);   /* PORT: src/host/w16_display.c */
extern int w16_script(void);   /* PORT: a lockstep or session run (src/host/w16_lockstep.c) */

static void water_cycle_tick(void)
{
    Palette *pal = (Palette *)&g_game_palette;
    if (g_game.game_flags & GAMEFLAG_NO_WATER_CYCLING)
        return;
    /* not in a scripted run: a session's screens are compared pixel for pixel with a baseline, and
       the water's phase follows the blink count, which a redraw's asynchronous window messages
       (the host's window system) shift by a blink now and then -- a third of the runs differed.
       The rules never see the water either way; a person checks it in play (2026-10-12). */
    if (w16_script())
        return;
    if (g_palette_mode == 1) {
        /* up by one, as palette_rotate(pal, 136, 6, +1) does, without its AnimatePalette */
        int k;
        unsigned char r = pal->e[141].r, g = pal->e[141].g, b = pal->e[141].b;
        for (k = 141; k > 136; k--)
            pal->e[k] = pal->e[k - 1];
        pal->e[136].r = r;
        pal->e[136].g = g;
        pal->e[136].b = b;
        MSUpdatePalette(pal, g_game_palette.hpal, 136, 6);
        w16_palette_quiet(136, 6, &pal->e[136]);
    } else {
        palette_rotate(pal, 136, 6, 1);
        if (++g_game_palette.seed == 0)        /* 0 would read as "never set" */
            ++g_game_palette.seed;
    }
    ShowMapRegion(g_view_x0, g_view_y0, g_view_w, g_view_h);
}

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
                    water_cycle_tick();   /* PORT: water colour cycling, the blink's tick (see above) */
                    MiniWin_HighlightActiveUnit((MiniWin *)&g_mini_win, g_blink = !g_blink);
                    ShowMapRegion(g_cursor_x, g_cursor_y, 1, 1);
                } else {
                    water_cycle_tick();   /* PORT: water colour cycling, the blink's tick (see above) */
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
