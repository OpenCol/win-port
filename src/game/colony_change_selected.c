/* colony_change_selected: candidate for 1010:4251 (259 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Refreshes the colony screen's panels for the colony window at SEG20:0x25be and, in the
 * tutorial (bit 7 of SEG20:0x7784) when the food lesson (bit 4 of 0x7782) has not been given
 * and the colony has a food surplus or shortfall (data segment 21 words 0x18c / 0x1b4),
 * shows TUTORIAL16 with the colony window's DialogWin part (0x25e4, window +0x26) disabled around it and marks the lesson given.
 *
 * RENAMED 2026-09-26 from tutorial_colony_food: the Mac build's colony_change_selected(void), bound by call-graph agreement (tools/mac_callgraph.py): 11 of its Mac call edges already bound land on this function (caller ColonyMouseMap; caller ColonyMouseUnits; caller KeyClick; callee DrawBuildings; callee DrawButtons), the runner-up gets 9, and the arity agrees (0). Its rival colony_window_refresh (9) is colony_change_mode instead: see that record.
 */

#include "colonization.h"

#include "gameflag.h"
#include "goods.h"

typedef struct { char pad[1]; } ColonyWin;

#include "classes.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */
extern int g_deficit[];   /* SEG21:0x018c */
extern int g_shortfall[];   /* SEG21:0x01b4 */
extern void compute_colony_dynamic(void);
extern void ColonyWin_DrawColonists(void *c, int redraw);
extern void ColonyWin_DrawBuildings(void *c, int redraw);
extern void ColonyWin_DrawColonyMap(void *c, int redraw);
extern void ColonyWin_DrawButtons(void *c, int a, int b);
extern void ColonyWin_DrawMiscellaneous(void *c, int redraw);
extern void ColonyWin_DrawColonyShips(void *c, int redraw);
extern void ColonyWin_DrawMarket(ColonyWin *c, int redraw);
extern void ColonyWin_DrawTitle(ColonyWin *c);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void win_disable_1010_e459(void *c);

extern void win_enable_1010_e4d4(void *c);
void colony_change_selected(void)
{
    g_panel_toggle = 1;
    compute_colony_dynamic();
    ColonyWin_DrawColonists(&g_colony_window, 1);
    ColonyWin_DrawBuildings(&g_colony_window, 1);
    ColonyWin_DrawColonyMap(&g_colony_window, 1);
    ColonyWin_DrawButtons(&g_colony_window, 1, 0);
    ColonyWin_DrawMiscellaneous(&g_colony_window, 1);
    ColonyWin_DrawColonyShips(&g_colony_window, 1);
    ColonyWin_DrawMarket((ColonyWin *)&g_colony_window, 1);
    ColonyWin_DrawTitle((ColonyWin *)&g_colony_window);
    if ((g_game.game_flags & GAMEFLAG_TUTORIAL) && !(g_game.tutorial_flags & 0x10) && (g_deficit[GOODS_FOOD] != 0 || g_shortfall[GOODS_FOOD] != 0)) {
        win_disable_1010_e459(&g_colony_win_rec);
        show_text_dialog_1010_e528("TUTORIAL16");
        win_enable_1010_e4d4(&g_colony_win_rec);
        g_game.tutorial_flags |= 0x10;
    }
}
