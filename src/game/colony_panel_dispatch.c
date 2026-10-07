/* colony_panel_dispatch: candidate for 1010:4616 (121 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: colony_panel_dispatch
 *
 * Routes a click by panel number after selecting the window's colony: panel
 * 4 goes to ColonyWin_ColonyMouseMisc, panel 0 to ColonyWin_ColonyMouseColonists; the others do nothing.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void set_colony(int colony);
extern void ColonyWin_ColonyMouseMisc(ColonyWin *w, int x, int y);
extern void ColonyWin_ColonyMouseColonists(ColonyWin *w, int x, int y);

void colony_panel_dispatch(ColonyWin *w, int panel, int x, int y)
{
    set_colony(w->colony);
    switch (panel) {
    case 5:
        break;
    case 7:
        break;
    case 8:
        break;
    case 4:
        ColonyWin_ColonyMouseMisc(w, x, y);
        break;
    case 0:
        ColonyWin_ColonyMouseColonists(w, x, y);
        break;
    case 2:
        break;
    case 1:
        break;
    default:
        break;
    }
}
