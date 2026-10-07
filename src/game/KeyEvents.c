/* KeyEvents: candidate for 1048:d006 (510 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The keyboard accelerator handler: one key code, 0x200 meaning Ctrl, dispatched with a guard
 * against re-entry in the byte at DGROUP:0x2012 and nothing done unless the game record at
 * SEG20:0x40d6 says a game is up. Ctrl+W, Ctrl+I, Ctrl+N in a row open the debug menu
 * (`StartCheat`); any other key breaks the sequence.
 *
 * Ctrl+Q asks "DOS" and sets the quit flag; Ctrl+R asks "RETIRE", records the score -- the hall of
 * fame entry only when bit 0x10 of SEG20:0x7784 is clear -- and quits. 0xb0 opens the Colonopedia
 * on the terrain under the cursor, if the viewing nation is not European, the square is explored
 * for it, or everything is revealed; 0xb1..0xb9 open reports 2..8, 1 and 9. H reveals the map, E
 * opens the European status, C recentres, V views the pieces, M either moves the cursor or selects
 * the next unit by the view mode, and Z and X zoom, updating the zoom menu items 0x130/0x131 when
 * the zoom reaches an end. Everything else goes to the map's own key handler.
 *
 * THE CASE LIST IS WRITTEN OUT, MOST OF IT EMPTY. The five jump tables cover 0x41-0x50, 0x53-0x58,
 * 0xa1-0xa4, 0xa7-0xb6 and 0xb8-0xc2, and 0x52, 0xa6, 0xc3, 0xd0 and 0xd1 are compared on their
 * own only to land on the default: the source listed keys that do nothing special, and those
 * labels are what shape the dispatch (rules 13 and 29). The bodies are in memory in the order the
 * source wrote them (rule 36).
 *
 * WHICH empty labels sit inside the tables is not recoverable, only roughly how many. Borland
 * splits a sparse switch at its median label, so the compares 0xa6, 0x52, 0x5a, 0xb7 and 0xc3
 * pin the counts on each side. 144 combinations of extra labels were graded and five matched,
 * all with three or four extra labels in 0x41-0x50 and one or two in each of 0x53-0x58, 0xa7-0xb6
 * and 0xb8-0xc2. This file uses the smallest of the five; the particular keys are a choice.
 *
 * RENAMED 2026-09-27 from keyboard_accelerator_handler: the Mac build's KeyEvents, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: The key dispatcher: StartCheat, Open/CloseBackdrop, FinalScore, PediaWin_Show, nine ReportWin_DoReport calls, RevealHiddenTerrain, EuropeWin_Show, zoom plus four MSEnableItem calls, GameBrains_KeyboardEvent. That is Mac KeyEvents' callee list, and both sides are called from the Pedia/Report KeyClick methods; 1 arg, ratio 2.46.
 */

#include "colonization.h"

#include "gameflag.h"
#include "w16types.h"
#include "classes.h"
/* DEFINED here: segment 10's _DATA holds it after MainWin_MainMenuProc's `filter` template
   and before the module's vtables (tools/check_module_data.py, 2026-10-02) */
char g_in_accel = 0;                       /* DGROUP:0x2012 */

#include "records.h"
extern GameBrains g_game_brains;            /* SEG20:0x40d6 */
extern GameState g_game;   /* SEG20:0x7782 */
extern char g_pedia_win;              /* SEG20:0x387c */

typedef struct { char pad[1]; } EuropeWin;

extern void OpenBackdrop(void);
extern void FinalScore(void);
extern void CloseBackdrop(void);
extern unsigned char site_get(int x, int y);

extern void PediaWin_Show(void *p, int kind, int article);

extern void MapWin_RevealHiddenTerrain(void *w);

extern void GameBrains_SetViewMode(void *rec);
extern void set_center(int x, int y, int flag);
extern int GameBrains_EndUnitTurn(void *rec, int force);
extern int MapWin_ZoomIn(void *w);
extern int MapWin_ZoomOut(void *w);
extern void GameBrains_KeyboardEvent(void *rec, int key);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int get_w_6(void *r);
extern void StartCheat(void);

extern void EuropeWin_Show(void *p, int nation, int unit);
extern void MSEnableItem_1048_e2dd(void *m, long id, int on);

void KeyEvents(int key)
{
    int zoom;

    if (g_in_accel != 0)
        return;
    g_in_accel = 1;
    if (get_w_6(&g_game_brains) == 0)
        goto done;
    if (key == 0x257)
        g_cheat_step = 1;
    else if (key == 0x249 && g_cheat_step == 1)
        g_cheat_step = 2;
    else if (key == 0x24e && g_cheat_step == 2)
        StartCheat();
    else
        g_cheat_step = 0;
    switch (key) {
    case 0x251:
        if (ask_text_resource("DOS") == 2)
            g_quit_flag = 1;
        break;
    case 0x252:
        if (ask_text_resource("RETIRE") == 2) {
            OpenBackdrop();
            if ((g_game.game_flags & GAMEFLAG_SCORED) == 0)
                FinalScore();
            g_game.game_in_progress = 0;
            g_quit_flag = 1;
            CloseBackdrop();
        }
        break;
    case 0xb0:
        if (g_game.view_nation >= 4
            || (site_get(g_cursor_x, g_cursor_y) & (0x10 << g_game.view_nation)) != 0
            || g_game.reveal_all != 0)
            PediaWin_Show((PediaWin *)&g_pedia_win, 2, terrain_at(g_cursor_x, g_cursor_y));
        break;
    case 0xb1:
        ReportWin_DoReport(&g_report_window, 2, 0);
        break;
    case 0xb2:
        ReportWin_DoReport(&g_report_window, 3, 0);
        break;
    case 0xb3:
        ReportWin_DoReport(&g_report_window, 4, 0);
        break;
    case 0xb4:
        ReportWin_DoReport(&g_report_window, 5, 0);
        break;
    case 0xb5:
        ReportWin_DoReport(&g_report_window, 6, 0);
        break;
    case 0xb6:
        ReportWin_DoReport(&g_report_window, 7, 0);
        break;
    case 0xb7:
        ReportWin_DoReport(&g_report_window, 8, 0);
        break;
    case 0xb8:
        ReportWin_DoReport(&g_report_window, 1, 0);
        break;
    case 0xb9:
        ReportWin_DoReport(&g_report_window, 9, 0);
        break;
    case 'H':
        MapWin_RevealHiddenTerrain(&g_map_window);
        break;
    case 'E':
        EuropeWin_Show(&g_europe_win, g_game.player_nation, -1);
        break;
    case 'C':
        MapWin_Recenter(&g_map_window, g_cursor_x, g_cursor_y);
        break;
    case 'V':
        GameBrains_SetViewMode(&g_game_brains);
        break;
    case 'M':
        if (g_game.view_mode == 0)
            set_center(g_cursor_x, g_cursor_y, 1);
        else
            GameBrains_EndUnitTurn(&g_game_brains, 0);
        break;
    case 'Z':
        zoom = MapWin_ZoomIn(&g_map_window);
        if (zoom == 4)
            MSEnableItem_1048_e2dd(&g_menu_owner, 0x130L, 0);
        else if (zoom == -3)
            MSEnableItem_1048_e2dd(&g_menu_owner, 0x131L, 1);
        break;
    case 'X':
        zoom = MapWin_ZoomOut(&g_map_window);
        if (zoom == -4)
            MSEnableItem_1048_e2dd(&g_menu_owner, 0x131L, 0);
        else if (zoom == 3)
            MSEnableItem_1048_e2dd(&g_menu_owner, 0x130L, 1);
        break;
    case 0x41: case 0x50: case 0x42: case 0x44: case 0x46: case 0x52: case 0x53: case 0x54:
    case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa6: case 0xa7: case 0xa8:
    case 0xba: case 0xc2: case 0xc3: case 0xd0: case 0xd1:
    default:
        GameBrains_KeyboardEvent(&g_game_brains, key);
        break;
    }
done:
    g_in_accel = 0;
}
