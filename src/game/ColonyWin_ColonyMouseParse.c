/* ColonyWin_ColonyMouseParse: candidate for 1010:4fd0 (187 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Routes a right click by panel number: 5 to ColonyWin_ColonyMouseMarket, 7 to
 * ColonyWin_ColonyMouseShipsCargo, 8 to ColonyWin_ColonyMouseShipsProper, 4 to ColonyWin_ColonyMouseMisc, 0 to
 * ColonyWin_ColonyMouseColonists, 2 to ColonyWin_ColonyMouseBuildings, 1 to ColonyWin_ColonyMouseMap; 3 and 6 do nothing.
 *
 * RENAMED 2026-09-27 from colony_panel_rclick: BEYOND THE PROPOSALS: the ColonyWin mouse block maps one to one (see chains). Mac ColonyWin::ColonyMouseParse(short, short, short), CODE 13 0x1d8e, 212 bytes. Its bsr calls are ColonyMouseMarket, ColonyMouseShipsCargo, ColonyMouseShipsProper, ColonyMouseMisc, ColonyMouseColonists, ColonyMouseBuildings and ColonyMouseMap, through a switch. 4fd0's calls are the same seven: colony_goods_click, colony_cargo_click, colony_dock_click, ColonyWin_ColonyMouseMisc, colony_row_click, colony_building_click, ColonyWin_ColonyMouseMap. Callers: Mac (bsr) LeftMouseClick, RightMouseClick, DblMouseClick; Windows ColonyWin_LeftMouseClick, _RightMouseClick, _DblMouseClick. Arity: this+3 shorts against (void *w, int panel, int x, int y). Layout: directly after KeyClick on both. Size 212 against 187. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

typedef struct { char pad[1]; } ColonyWin;
extern void ColonyWin_ColonyMouseMarket(void *w, int x, int y);
extern void ColonyWin_ColonyMouseShipsCargo(void *w, int x, int y);
extern void ColonyWin_ColonyMouseShipsProper(ColonyWin *w, int x, int y);
extern void ColonyWin_ColonyMouseMisc(ColonyWin *w, int x, int y);
extern void ColonyWin_ColonyMouseColonists(ColonyWin *w, int x, int y);
extern void ColonyWin_ColonyMouseBuildings(ColonyWin *w, int x, int y);
extern void ColonyWin_ColonyMouseMap(void *w, int x, int y);

void ColonyWin_ColonyMouseParse(ColonyWin *w, int panel, int x, int y)
{
    switch (panel) {
    case 5:
        ColonyWin_ColonyMouseMarket(w, x, y);
        break;
    case 7:
        ColonyWin_ColonyMouseShipsCargo(w, x, y);
        break;
    case 8:
        ColonyWin_ColonyMouseShipsProper(w, x, y);
        break;
    case 4:
        ColonyWin_ColonyMouseMisc(w, x, y);
        break;
    case 0:
        ColonyWin_ColonyMouseColonists(w, x, y);
        break;
    case 2:
        ColonyWin_ColonyMouseBuildings(w, x, y);
        break;
    case 1:
        ColonyWin_ColonyMouseMap(w, x, y);
        break;
    default:
        break;
    }
}
