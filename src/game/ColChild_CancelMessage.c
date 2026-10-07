/* ColChild_CancelMessage: candidate for 1048:10c6 (41 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_CancelMessage
 *
 * Clears the two message flags at +0x75e/+0x75f and empties the text at +0x149.
 *
 * `a` is a SECOND PARAMETER that the body never reads, so nothing in these 41 bytes can
 * show it (§119). Thirty-nine call sites in the corpus push it -- `ColChild_CancelIfActive` and `EuropeWin_Show` (twice)
 * with 0, thirty-six message builders with 1 -- and all reach this address, so the prototype has it.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void ColChild_CancelMessage(ColChild *child, int unused1)
{
    child->active = 0;
    child->kind = 0;
    child->own_msg[0] = 0;
}
