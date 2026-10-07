/* StatWin_LeftMouseClick: candidate for 1058:8b67 (223 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: StatWin_LeftMouseClick
 * name-source: sibling-build
 *
 * A click inside the info window's hot rectangle (+0x76a..+0x770 of the owner): the owner's
 * window is notified through win_show_and_focus_1058_bc47; while a message is pending (SEG20:0x77c8) the flag at
 * SEG20:0x77c6 is cleared instead; in mode 1 (SEG20:0x7792) the piece record is handled by
 * GameBrains_EndUnitTurn, otherwise GameBrains_SetViewMode runs and the info window is repainted.
 *
 * RENAMED 2026-09-26 from on_info_window_pointer. It was called a pointer handler because it is
 * installed on handler slot +4, taken then to be the pointer slot; the window procedure's shipped
 * compare chain sends WM_LBUTTONDOWN to +4, and the description above was already of a click.
 *
 * RENAMED 2026-09-26 from on_info_window_click: the Mac build's StatWin::LeftMouseClick(short, short): StatWin_Init installs it as the window's lbutton_down handler, and it takes the same two shorts.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);
#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */
extern GameBrains g_game_brains;     /* SEG20:0x40d6 */
/* same object: 4-byte `push cs; call ` (rule 47) */

extern int GameBrains_EndUnitTurn(void *rec, int force);
extern void GameBrains_SetViewMode(void *rec);
extern void UpdateStats(void);

void StatWin_LeftMouseClick(int x, int y)
{
    StatWin *o;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (StatWin *)w;
    if (o->rect.left < x && o->rect.right > x && o->rect.top < y && o->rect.bottom > y) {
        win_show_and_focus_1058_bc47(&(DialogWin &)*o);
        if (g_game.end_of_turn_pending != 0) {
            g_game.human_turn_active = 0;
            return;
        }
        if (g_game.view_mode == 1) {
            GameBrains_EndUnitTurn(&g_game_brains, 0);
            return;
        }
        GameBrains_SetViewMode(&g_game_brains);
        UpdateStats();
    }
}
