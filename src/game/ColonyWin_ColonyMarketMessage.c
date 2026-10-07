/* ColonyWin_ColonyMarketMessage: candidate for 1010:b11a (131 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_ColonyMarketMessage
 * name-source: sibling-build
 *
 * "goods X on/off" on the colony window: goods text 0xf or 0x10 by `flag`,
 * then the goods' own name, shown as kind 1 with no timeout.  The two
 * helpers are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-27 from goods_toggle_msg: Mac ColonyWin::ColonyMarketMessage(short, short, short), 150 bytes, against 131. Callers: ColonyMouseMarket and ColonyMouseShipsCargo on both. Mac seq set_colony, CancelMessage, ColonyMessage x2, AddToMessage, ColonyActivateMessage is the Windows callee set (b08e x2, b048). Arity: this+3 = (w, goods, unused, flag). Layout: the Mac CODE 30 run Close, Done, DrawButtons, ColonyActivateMessage, ColonyMessage, ColonyMarketMessage, AttemptCargoToShip, AttemptCargoToColony, AttemptShipToShip, LoadColony, UpdateColony is the Windows 1010 run ColonyWin_Close, Done, DrawButtons, b048, b08e, [colony_goods_message, Windows-only], b11a, b19d, b508, ColonyWin_AttemptShipToShip, ColonyWin_LoadColony, bd3f.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern int g_stub_a;
extern void set_colony(int colony);
extern "C" void ColChild_CancelMessage(ColChild *child, int a);   /* PORT: C linkage, as defined (W2) */
extern "C" void ColChild_AddToMessage(ColChild *child, int idx);   /* PORT: C linkage, as defined (W2) */

extern "C" void ColonyWin_ColonyMessage(ColonyWin *w, int n);
extern "C" void ColonyWin_ColonyActivateMessage(ColonyWin *w, int kind, long ms);

void ColonyWin_ColonyMarketMessage(ColonyWin *w, int goods, int unused, int flag)
{
    set_colony(w->colony);
    ColChild_CancelMessage((ColChild *)w, 1);
    if (flag != 0)
        ColonyWin_ColonyMessage(w, 0xf);
    else
        ColonyWin_ColonyMessage(w, 0x10);
    ColChild_AddToMessage((ColChild *)w, g_goods_name_word[goods]);
    ColonyWin_ColonyActivateMessage(w, 1, 0L);
}
