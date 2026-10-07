/* colony_change_mode: candidate for 1010:41be (147 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Redraws every panel of the colony window (SEG20:0x25be) with the
 * `redraw` argument set: compute_colony_dynamic first, then the eight panel
 * painters in turn, the title line last; there is no separate refresh call.  Called after cargo has been moved into the warehouse.
 *
 * RENAMED 2026-09-26 from colony_window_refresh: the Mac build's colony_change_mode(void), bound by call-graph agreement (tools/mac_callgraph.py) jointly with colony_change_selected: the two Mac functions call the same eight ColonyWin Draw* methods and compute_colony_dynamic, and so do the two Windows ones, but colony_change_mode's callers are the ship-click and cargo paths (colony_click_ship, attempt_cargo_to_colony/_to_ship, colony_hammer_buy) -- this function's -- while colony_change_selected's are the mouse, key and profession-popup paths, which are tutorial_colony_food's.
 */

#include "colonization.h"

typedef struct { char pad[1]; } ColonyWin;

extern void compute_colony_dynamic(void);
extern void ColonyWin_DrawColonists(void *c, int redraw);
extern void ColonyWin_DrawBuildings(void *c, int redraw);
extern void ColonyWin_DrawColonyMap(void *c, int redraw);
extern void ColonyWin_DrawButtons(void *c, int a, int b);
extern void ColonyWin_DrawMiscellaneous(void *c, int redraw);
extern void ColonyWin_DrawColonyShips(void *c, int redraw);
extern void ColonyWin_DrawMarket(ColonyWin *c, int redraw);
extern void ColonyWin_DrawTitle(ColonyWin *c);

void colony_change_mode(void)
{
    compute_colony_dynamic();
    ColonyWin_DrawColonists(&g_colony_window, 1);
    ColonyWin_DrawBuildings(&g_colony_window, 1);
    ColonyWin_DrawColonyMap(&g_colony_window, 1);
    ColonyWin_DrawButtons(&g_colony_window, 1, 0);
    ColonyWin_DrawMiscellaneous(&g_colony_window, 1);
    ColonyWin_DrawColonyShips(&g_colony_window, 1);
    ColonyWin_DrawMarket((ColonyWin *)&g_colony_window, 1);
    ColonyWin_DrawTitle((ColonyWin *)&g_colony_window);
}
