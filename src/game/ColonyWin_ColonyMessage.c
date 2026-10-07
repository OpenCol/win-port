/* ColonyWin_ColonyMessage: candidate for 1010:b08e (59 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_ColonyMessage
 * name-source: sibling-build
 *
 * Selects the window's colony and appends message fragment g_colony_message_words[n] to its message.
 * Near-called from ColonyWin_AttemptCargoToColony.
 *
 * RENAMED 2026-09-26 from colony_add_goods_text: Selects the window's colony and appends g_colony_message_words[n] (SEG20:0xd996, the colony message-fragment table colony_message/colony_error also index) to the window's message. It does not add goods text: goods names come from the separate g_goods_name_word table (SEG20:0xd86e), and ColonyWin_AttemptCargoToShip calls it with 6 and 7 around a separate ColChild_AddToMessage(goods name), so the old name makes the reader think the goods name is added twice.
 *
 * RENAMED 2026-09-27 from colony_add_message_text: Mac ColonyWin::ColonyMessage(short), 76 bytes, against 59. The callers are the same set as b048's. Callees: set_colony and ColChild::AddToMessage on both. Arity: this+short. Layout: the Mac CODE 30 run Close, Done, DrawButtons, ColonyActivateMessage, ColonyMessage, ColonyMarketMessage, AttemptCargoToShip, AttemptCargoToColony, AttemptShipToShip, LoadColony, UpdateColony is the Windows 1010 run ColonyWin_Close, Done, DrawButtons, b048, b08e, [colony_goods_message, Windows-only], b11a, b19d, b508, ColonyWin_AttemptShipToShip, ColonyWin_LoadColony, bd3f.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void set_colony(int colony);
extern void ColChild_AddToMessage(ColChild *child, int idx);

void ColonyWin_ColonyMessage(ColonyWin *w, int n)
{
    set_colony(w->colony);
    ColChild_AddToMessage((ColChild *)w, g_colony_message_words[n]);
}
