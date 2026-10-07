/* PORT: this file's own declarations of ColChild_ActivateMessage, ColChild_DrawAnyMessage removed: the header declares them from the definition (W1 step 3). */
/* ColonyWin_ColonyActivateMessage: candidate for 1010:b048 (70 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_ColonyActivateMessage
 * name-source: sibling-build
 *
 * Selects the window's colony, arms its message of the given kind for `ms`
 * and draws it at once.  Near-called from ColonyWin_AttemptCargoToColony.
 *
 * RENAMED 2026-09-27 from colony_show_message: Mac ColonyWin::ColonyActivateMessage(short, long), 100 bytes, against 70. Callers: the Mac AttemptCargoToColony, AttemptCargoToShip, AttemptShipToShip and ColonyMarketMessage are b508, b19d, b882 and b11a here (plus the Windows-only colony_goods_message). Callees: set_colony, ColChild::ActivateMessage and DrawAnyMessage, in that order, on both. Arity: this+short+long = 3 = (w, kind, long ms). Layout: the Mac CODE 30 run Close, Done, DrawButtons, ColonyActivateMessage, ColonyMessage, ColonyMarketMessage, AttemptCargoToShip, AttemptCargoToColony, AttemptShipToShip, LoadColony, UpdateColony is the Windows 1010 run ColonyWin_Close, Done, DrawButtons, b048, b08e, [colony_goods_message, Windows-only], b11a, b19d, b508, ColonyWin_AttemptShipToShip, ColonyWin_LoadColony, bd3f.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void set_colony(int colony);

void ColonyWin_ColonyActivateMessage(ColonyWin *w, int kind, long ms)
{
    set_colony(w->colony);
    ColChild_ActivateMessage(w, kind, ms);
    ColChild_DrawAnyMessage(w, 1, 0L);
}
