/* PORT: this file's own declarations of ColonyWin_UpdateColony removed: the header declares it from the definition (W1 step 3). */
/* ColonyWin_DrawColony: candidate for 1010:aed6 (211 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DrawColony
 * name-source: sibling-build
 *
 * Redraws the whole colony window: select its colony, recount
 * (compute_colony_parameters), sort the colonists, then the backdrop copy (the full
 * 640x440 cell) and the panel painters in turn with redraw 0.  The nine
 * -called painters are defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from colony_window_redraw: the Mac build's ColonyWin::DrawColony(void): this only, repainting the whole colony window panel by panel.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern int g_stub_a;
extern void set_colony(int colony);
extern void compute_colony_parameters(void);
extern void sort_colonists_by_profession(void);
extern void ColonyWin_DrawButtons(void *w, int a, int b);

extern "C" void ColonyWin_GenerateColonyBaseMap(ColonyWin *w);
extern "C" void ColonyWin_DrawTitle(ColonyWin *w);
extern "C" void ColonyWin_DrawColonyMap(ColonyWin *w, int redraw);
extern "C" void ColonyWin_DrawColonists(ColonyWin *w, int redraw);
extern "C" void ColonyWin_DrawMarket(ColonyWin *w, int redraw);
extern "C" void ColonyWin_DrawColonyShips(ColonyWin *w, int redraw);
extern "C" void ColonyWin_DrawMiscellaneous(ColonyWin *w, int redraw);
extern "C" void ColonyWin_DrawBuildings(ColonyWin *w, int redraw);

void ColonyWin_DrawColony(ColonyWin *w)
{
    set_colony(w->colony);
    compute_colony_parameters();
    sort_colonists_by_profession();
    ColonyWin_GenerateColonyBaseMap(w);
    ColonyWin_UpdateColony(w, 0, 0, 0x280, 0x1b8);
    ColonyWin_DrawTitle(w);
    ColonyWin_DrawColonyMap(w, 0);
    ColonyWin_DrawColonists(w, 0);
    ColonyWin_DrawMarket(w, 0);
    ColonyWin_DrawButtons(w, 0, 0);
    ColonyWin_DrawColonyShips(w, 0);
    ColonyWin_DrawMiscellaneous(w, 0);
    ColonyWin_DrawBuildings(w, 0);
    copy_all_to_screen_1010_e3e6(w);
}
