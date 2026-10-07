/* colony_goods_message: candidate for 1010:b0c9 (81 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: colony_goods_message
 *
 * Shows goods message n on the colony window for `ms`: select the colony,
 * cancel the current message, add the goods text, show as kind 3.  The two
 * helpers are -called and defined in-file as stubs.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern int g_stub_a;
extern void set_colony(int colony);
extern "C" void ColChild_CancelMessage(ColChild *child, int a);   /* PORT: C linkage, as defined (W2) */

extern "C" void ColonyWin_ColonyMessage(ColonyWin *w, int n);
extern "C" void ColonyWin_ColonyActivateMessage(ColonyWin *w, int kind, long ms);

void colony_goods_message(ColonyWin *w, int n, long ms)
{
    set_colony(w->colony);
    ColChild_CancelMessage((ColChild *)w, 1);
    ColonyWin_ColonyMessage(w, n);
    ColonyWin_ColonyActivateMessage(w, 3, ms);
}
