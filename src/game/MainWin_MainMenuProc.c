/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 187 of the imported file. */
/* MainWin_MainMenuProc: candidate for 1048:c0b2 (2990 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MainWin_MainMenuProc
 * name-source: sibling-build
 * size-source: 2990, not the census 112. The census stops at the first unconditional jump out of
 * the switch; the function runs to 1048:cc7f and its last 433 bytes are the switch's two tables.
 *
 * THE MENU. Every WM_COMMAND the game window receives arrives here as a long, and this is the whole
 * menu bar in one function: File, View, Orders, Reports, Trade Routes, Colopedia, the Debug menu and
 * the three window toggles at the bottom. It does nothing at all unless the game context at
 * SEG20:0x40d6 reports a live window, so commands that arrive between games are dropped.
 *
 * SEVENTY-TWO COMMANDS, grouped by their hundreds: 55 (the control panel), 100..108 (File: 100
 * game options, 101 colony options, 102 sound, 103 music, 104 save, ... -- the shipped target
 * table at 1048:cbd0 pairs them so, per tools/check_data_refs.py; the linear-search switch's
 * target table is masked by the grader, so a rotated pairing graded clean until 2026-09-28.
 * The case-value table is SORTED by Borland but the arms are emitted in source order, and the
 * shipped arm order is options, colony options, sound, music, control panel, save: `case 55`
 * is written after `case 103`), 300..312
 * (View), 400..416 (Orders -- each one a key the unit handler already understands, so they all
 * funnel into the same keystroke entry point with a character code), 500..509 (Reports), 600..602
 * (Trade Routes), 700..706 (Colopedia), 800..811 (Debug) and 1000..1002 (the window toggles).
 *
 * THE SAVE PATH is the only one that builds anything. `filter` is an OFN-style filter block, four
 * NUL-terminated strings back to back, and its second string starts life as the literal
 * "2345678.SAV" only so that the season-and-year stamp can be copied over it in place -- the
 * default file name is the date. Note the deliberately wrong-looking first character: the copy
 * lands at `filter[11]`, one past "Saved Game", so "2345678.SAV" is a placeholder of exactly the
 * right length and never reaches the dialog.
 *
 * RETIRE (107) opens the overlay, records a Hall of Fame entry unless a final score has already
 * been recorded (SEG20:0x7784 bit 4, GAMEFLAG_SCORED), then sets the quit flag; DOS (108) just asks and
 * quits. The zoom commands (304..308) are the only ones that talk back to the menu: MapWin's
 * zoom in/out return the new level, and hitting either end greys out that item and re-enables
 * the other.
 *
 * 802 (Set View) has a shipped oddity: it computes `n` (6 when everything is revealed or a nation
 * is being watched, else 5) and then never uses it -- the SETVIEW prompt is asked unconditionally
 * and its answer alone decides. Answer 5 means "reveal all", anything below it selects nation
 * answer-1, and revealing all clears the high bit of SEG20:0x7784.
 *
 * 805 walks the scripted late-game events in order: raise the rebel sentiment at SEG20:0x77d2 to 75 and, while SEG20:0x77d4
 * is still negative, fire the Spanish Succession, then the Declaration, then the Intervention, and once both have happened it sets the
 * forced-war bit and shows FORCED.
 *
 * WHAT THE BYTES SETTLED: the switch is on a LONG, so Borland compiles it as a linear search over a
 * 288-byte table of 32-bit case values followed by a 144-byte table of targets, never a dense jump
 * table (rule 80) -- which is why the census reads only 112 bytes. That dword table is word-aligned,
 * so one zero byte sits between the RETF at 1048:caae and the table at 1048:cab0; only `-a` (word
 * alignment, the IDE's default project setting) makes the compiler emit it. Every code byte is
 * identical either way. The early bail-out is a `goto`, not a `return`: the shipped jumps to the
 * shared epilogue rather than inlining a second one (rule 12).
 * *
 * `g_panel_shown_mask` (DGROUP:0x4b2c) is which of the three panels are on screen: bit 1 the
 * map, bit 2 the world overview, bit 4 the status line. Menu commands 1000..1002 toggle a
 * bit, open or hide the window, and rewrite the menu item between "Hide X" and "Show X".
 * `run_game` sets it to 7 at startup, so all three begin shown.
 *
 * THE THREE `_dlg` GLOBALS ARE THE SAME WINDOWS, +0x26. `g_map_window_dlg` is
 * `g_map_window` + 0x26, `g_mini_win_dlg` is `g_mini_win` + 0x26 and
 * `g_info_win_dlg` is `g_stat_win` + 0x26 -- the DialogWin sub-object the rest of the
 * corpus reaches by writing `(char *)o + 0x26`. That is why the show arm of each
 * command here passes the window and the hide arm passes the `_dlg`: one takes the game
 * window and one takes the MS window inside it, and they are one object.
 *
 * RENAMED 2026-09-26 from wm_command_dispatcher: the Mac build's MainWin::MainMenuProc(long): the main screen's menu-command handler, installed as the command handler and taking the command as a long. The width matches.
 */

#include "colonization.h"

#include "gameflag.h"
#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
class PediaWin { public: char body[0x3b0c - 0x32b2]; PediaWin(); ~PediaWin(); };
class MainWin : public EventWin { public: MenuClass menu; MapWin map; MiniWin mini; StatWin stat; ReportWin report; ColonyWin colony; EuropeWin europe; PediaWin pedia; MainWin(); ~MainWin(); };
class GameWin;

extern char g_mini_win;              /* SEG20:0xde0 */

extern StatWin g_stat_win;            /* SEG20:0x1566 */

#include "records.h"
extern GameBrains g_game_brains;                 /* SEG20:0x40d6 */
extern GameState g_game;   /* SEG20:0x7782 */

extern void *GetEventWindow(void);
extern void BuildSaveGameName(char *dst);

extern int save_game(char *path);

extern void liberty_declared(void);
extern void FinalScore(void);
extern void set_center(int x, int y, int flag);
extern int GameBrains_EndUnitTurn(void *brains, int force);
extern void GameBrains_SetViewMode(void *ctx);
extern int MapWin_ZoomIn(void *w);
extern int MapWin_ZoomOut(void *w);
extern void MapWin_SetZoom(void *w, int z);
extern void MapWin_RevealHiddenTerrain(void *w);

extern void GameBrains_KeyboardEvent(void *rec, int key);
extern void GameBrains_NextUnitTurn(void *ctx);
extern unsigned char site_get(int x, int y);

extern void PediaWin_Show(void *p, int kind, int article);

extern void AboutBox(void);
extern void conquest(void);
extern void revolution(void);

extern void DrawMap(int flag);
extern void GameWin_Redraw(GameWin *w);
extern int popup_ask_number(char *ctx, char *name, int n);
extern void sound_play(int id);
extern void art_cheat(void);
extern void OpenBackdrop(void);
extern void CloseBackdrop(void);
extern void MainWin_DefaultDesktop(void *o);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int get_w_6(void *r);
extern "C" void game_menu_options(void);

extern "C" void game_menu_sound(void);
extern "C" void game_menu_music(void);
extern "C" void DoControlPanel(void);

extern "C" void EuropeWin_Show(void *p, int nation, int unit);
extern "C" void find_city(void);
extern "C" void MSEnableItem_1048_e2dd(void *m, unsigned long id, char enable);
extern "C" void orders_route(void);
extern "C" void orders_europe(void);
extern "C" void CheatMenuColonist(void);
extern "C" void game_menu_debug(void);
extern "C" void CheatMenuHuman(void);
extern "C" void CheatMenuBlow(void);

extern "C" void strategy_routine(void);
extern "C" void site_routine(void);

extern "C" void MSModifyMenu_1048_e300(void *m, long id, char *text);
extern "C" void MSHideWindow_of_1048_d4d5(Screen *o);
extern "C" void show_window_1048_dd42(void *w);

#pragma option -a
void MainWin_MainMenuProc(long cmd)
{
    MainWin *w;
    char *p;
    int zoom;
    int terrain;
    int n;
    int ans;
    char path[256];
    char date[80];
    char msg[80];
    char filter[52] = "Saved Game\0" "2345678.SAV\0" "Previously Saved Game\0" "*.sav\0";

    if (get_w_6(&g_game_brains) == 0)
        goto out;
    w = (MainWin *)(EventWin *)GetEventWindow();
    switch (cmd) {
    case 100:
        game_menu_options();
        break;
    case 101:
        game_colony_options();
        break;
    case 102:
        game_menu_sound();
        break;
    case 103:
        game_menu_music();
        break;
    case 55:
        DoControlPanel();
        break;
    case 104:
        BuildSaveGameName(date);
        msg[0] = 0;
        p = &filter[11];
        STRCPY(p, date);
        path[0] = 0;
        if (file_save_dialog(path, filter, (void*)("sav"), 0))
            save_game(path);
        break;
    case 105:
        load_game();
        break;
    case 106:
        liberty_declared();
        break;
    case 107:
        if (ask_text_resource("RETIRE") == 2) {
            OpenBackdrop();
            if (!(g_game.game_flags & GAMEFLAG_SCORED))
                FinalScore();
            g_game.game_in_progress = 0;
            g_quit_flag = 1;
            CloseBackdrop();
        }
        break;
    case 108:
        if (ask_text_resource("DOS") == 2)
            g_quit_flag = 1;
        break;
    case 300:
        if (g_game.view_mode == 0)
            set_center(g_cursor_x, g_cursor_y, 1);
        else
            GameBrains_EndUnitTurn(&g_game_brains, 0);
        break;
    case 301:
        GameBrains_SetViewMode(&g_game_brains);
        break;
    case 302:
        EuropeWin_Show(&w->europe, g_game.player_nation, -1);
        break;
    case 303:
        find_city();
        break;
    case 304:
        zoom = MapWin_ZoomIn(&w->map);
        if (zoom == 4)
            MSEnableItem_1048_e2dd(&w->menu, 0x130L, 0);
        else if (zoom == -3)
            MSEnableItem_1048_e2dd(&w->menu, 0x131L, 1);
        break;
    case 305:
        zoom = MapWin_ZoomOut(&w->map);
        if (zoom == -4)
            MSEnableItem_1048_e2dd(&w->menu, 0x131L, 0);
        else if (zoom == 3)
            MSEnableItem_1048_e2dd(&w->menu, 0x130L, 1);
        break;
    case 306:
        MapWin_SetZoom(&w->map, 4);
        zoom = 4;
        MSEnableItem_1048_e2dd(&w->menu, 0x130L, 0);
        MSEnableItem_1048_e2dd(&w->menu, 0x131L, 1);
        break;
    case 307:
        MapWin_SetZoom(&w->map, 0);
        zoom = 0;
        MSEnableItem_1048_e2dd(&w->menu, 0x130L, 1);
        MSEnableItem_1048_e2dd(&w->menu, 0x131L, 1);
        break;
    case 308:
        MapWin_SetZoom(&w->map, -4);
        zoom = -4;
        MSEnableItem_1048_e2dd(&w->menu, 0x130L, 1);
        MSEnableItem_1048_e2dd(&w->menu, 0x131L, 1);
        break;
    case 310:
        MapWin_RevealHiddenTerrain(&w->map);
        break;
    case 311:
        MapWin_Recenter(&w->map, g_cursor_x, g_cursor_y);
        break;
    case 312:
        MainWin_DefaultDesktop(w);
        break;
    case 400:
        GameBrains_KeyboardEvent(&g_game_brains, 0x41);
        break;
    case 401:
        GameBrains_KeyboardEvent(&g_game_brains, 0x57);
        break;
    case 402:
        GameBrains_KeyboardEvent(&g_game_brains, 0x46);
        break;
    case 404:
        GameBrains_KeyboardEvent(&g_game_brains, 0x53);
        break;
    case 405:
        GameBrains_KeyboardEvent(&g_game_brains, 0x42);
        break;
    case 406:
        GameBrains_KeyboardEvent(&g_game_brains, 0x50);
        break;
    case 407:
        GameBrains_KeyboardEvent(&g_game_brains, 0x52);
        break;
    case 408:
        GameBrains_KeyboardEvent(&g_game_brains, 0x4c);
        break;
    case 409:
        GameBrains_KeyboardEvent(&g_game_brains, 0x55);
        break;
    case 411:
        GameBrains_KeyboardEvent(&g_game_brains, 0x47);
        break;
    case 412:
        orders_route();
        GameBrains_NextUnitTurn(&g_game_brains);
        break;
    case 413:
        orders_europe();
        GameBrains_NextUnitTurn(&g_game_brains);
        break;
    case 414:
        GameBrains_KeyboardEvent(&g_game_brains, 0xd1);
        break;
    case 415:
        GameBrains_KeyboardEvent(&g_game_brains, 0x4f);
        break;
    case 416:
        GameBrains_KeyboardEvent(&g_game_brains, 0x144);
        break;
    case 500:
        if (g_game.view_nation >= 4
            || (site_get(g_cursor_x, g_cursor_y) & (0x10 << g_game.view_nation))
            || g_game.reveal_all != 0) {
            terrain = terrain_at(g_cursor_x, g_cursor_y);
            PediaWin_Show(&w->pedia, 2, terrain);
        }
        break;
    case 501:
        ReportWin_DoReport(&w->report, 2, 0);
        break;
    case 502:
        ReportWin_DoReport(&w->report, 3, 0);
        break;
    case 503:
        ReportWin_DoReport(&w->report, 4, 0);
        break;
    case 504:
        ReportWin_DoReport(&w->report, 5, 0);
        break;
    case 505:
        ReportWin_DoReport(&w->report, 6, 0);
        break;
    case 506:
        ReportWin_DoReport(&w->report, 7, 0);
        break;
    case 507:
        ReportWin_DoReport(&w->report, 8, 0);
        break;
    case 508:
        ReportWin_DoReport(&w->report, 1, 0);
        break;
    case 509:
        ReportWin_DoReport(&w->report, 9, 0);
        break;
    case 600:
        route_edit(-1);
        break;
    case 601:
        route_create();
        break;
    case 602:
        route_delete();
        break;
    case 700:
        PediaWin_Show(&w->pedia, 0, -1);
        break;
    case 701:
        PediaWin_Show(&w->pedia, 1, -1);
        break;
    case 702:
        PediaWin_Show(&w->pedia, 2, -1);
        break;
    case 703:
        PediaWin_Show(&w->pedia, 3, -1);
        break;
    case 704:
        PediaWin_Show(&w->pedia, 4, -1);
        break;
    case 705:
        PediaWin_Show(&w->pedia, 5, -1);
        break;
    case 706:
        AboutBox();
        break;
    case 800:
        CheatMenuColonist();
        break;
    case 801:
        game_menu_debug();
        break;
    case 802:
        if (g_game.reveal_all != 0 || g_game.view_override >= 0)
            n = 6;
        else
            n = 5;
        g_popup_dude = 3;
        ans = ask_text_resource("SETVIEW");
        g_game.reveal_all = ans == 5;
        g_game.view_override = ans < 5 ? ans - 1 : -1;
        if (g_game.reveal_all != 0)
            g_game.game_flags &= 0x7fff;
        if (g_game.view_override >= 0)
            g_game.view_nation = g_game.view_override;
        DrawMap(1);
        GameWin_Redraw((GameWin *)&g_mini_win);
        break;
    case 803:
        CheatMenuHuman();
        break;
    case 804:
        CheatMenuBlow();
        break;
    case 805:
        if (g_game.rebel_sentiment < 0x4b) {
            g_game.rebel_sentiment = 0x4b;
            if (g_game.mother_country_idx < 0)
                conquest();
        } else if (!(g_game.game_flags & GAMEFLAG_INDEPENDENCE))
            revolution();
        else if (!(g_game.game_flags & GAMEFLAG_INTERVENTION))
            intervention();
        else {
            g_game.game_flags |= 0x20;
            ask_text_resource("FORCED");
        }
        break;
    case 806:
        if (popup_ask_number("DEBUG", "SOUND", 1) == 0)
            sound_play((int)g_text_args[0]);
        break;
    case 807:
        memory_check();
        break;
    case 808:
        strategy_routine();
        break;
    case 809:
        site_routine();
        break;
    case 810:
        test_routine();
        break;
    case 811:
        art_cheat();
        break;
    case 1000:
        if (!((int)g_panel_shown_mask & 1)) {
            show_window_1048_dd42((GameWin *)&g_map_window);
            MSModifyMenu_1048_e300(&g_menu_owner, 0x3e8L, "Hide Map");
            g_panel_shown_mask |= 1;
        } else {
            MSHideWindow_of_1048_d4d5((Screen *)&g_map_window_dlg);
            MSModifyMenu_1048_e300(&g_menu_owner, 0x3e8L, "Show Map");
            g_panel_shown_mask &= ~1;
        }
        break;
    case 1001:
        if (!((int)g_panel_shown_mask & 2)) {
            show_window_1048_dd42((GameWin *)&g_mini_win);
            MSModifyMenu_1048_e300(&g_menu_owner, 0x3e9L, "Hide World");
            g_panel_shown_mask |= 2;
        } else {
            MSHideWindow_of_1048_d4d5((Screen *)&g_mini_win_dlg);
            MSModifyMenu_1048_e300(&g_menu_owner, 0x3e9L, "Show World");
            g_panel_shown_mask &= ~2;
        }
        break;
    case 1002:
        if (!((int)g_panel_shown_mask & 4)) {
            show_window_1048_dd42((GameWin *)&g_stat_win);
            MSModifyMenu_1048_e300(&g_menu_owner, 0x3eaL, "Hide Status");
            g_panel_shown_mask |= 4;
        } else {
            MSHideWindow_of_1048_d4d5((Screen *)&g_info_win_dlg);
            MSModifyMenu_1048_e300(&g_menu_owner, 0x3eaL, "Show Status");
            g_panel_shown_mask &= ~4;
        }
        break;
    }
out:
    ;
}
