/* ColChild_ActivateMessage: candidate for 1048:126c (34 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_ActivateMessage
 *
 * Marks the colony child's message active (+0x75e) with its kind byte (+0x75f); the third
 * argument is passed on by the callers but unused here.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void ColChild_ActivateMessage(ColChild *child, int kind, long unused1)
{
    child->active = 1;
    child->kind = kind;
}
