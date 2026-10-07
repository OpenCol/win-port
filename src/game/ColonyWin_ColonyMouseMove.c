/* ColonyWin_ColonyMouseMove: candidate for 1010:508c (127 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: ColonyWin_ColonyMouseMove
 *
 * Handles a click on a colony panel, guarded against re-entry by the byte
 * at DGROUP:0x9fc: panel 5 to ColonyWin_ColonyMouseMarket, 2 to ColonyWin_ColonyMouseBuildings, 1 to
 * ColonyWin_ColonyMouseMap, anything else closes the popup (label_remove_label, -called
 * and defined in-file as a stub).
 *
 * RENAMED 2026-09-27 from colony_panel_click: BEYOND THE PROPOSALS. Mac ColonyWin::ColonyMouseMove(short, short, short), CODE 13 0x1e62, 172 bytes: label_remove_label, then bsr ColonyMouseMarket, ColonyMouseBuildings, ColonyMouseMap. 508c calls label_remove_label, colony_goods_click, colony_building_click, ColonyWin_ColonyMouseMap. Caller: Windows ColonyWin_MouseMove; the Mac MouseMove reaches it the same way. Arity: this+3 against (w, panel, x, y). Layout: second after KeyClick on both. Size 172 against 127. Distinct from the existing ColonyWin_MouseMove, also case-insensitively. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

typedef struct { char pad[1]; } ColonyWin;

extern int g_stub_a;
extern void ColonyWin_ColonyMouseMarket(void *w, int x, int y);
extern void ColonyWin_ColonyMouseBuildings(ColonyWin *w, int x, int y);
extern void ColonyWin_ColonyMouseMap(void *w, int x, int y);

extern void label_remove_label(void);

void ColonyWin_ColonyMouseMove(ColonyWin *w, int panel, int x, int y)
{
    if (g_panel_click_busy != 0)
        return;
    g_panel_click_busy = 1;
    switch (panel) {
    case 5:
        ColonyWin_ColonyMouseMarket(w, x, y);
        break;
    case 2:
        ColonyWin_ColonyMouseBuildings(w, x, y);
        break;
    case 1:
        ColonyWin_ColonyMouseMap(w, x, y);
        break;
    default:
        label_remove_label();
        break;
    }
    g_panel_click_busy = 0;
}
