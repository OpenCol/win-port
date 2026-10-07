/* ColChild_BackspaceMessage: candidate for 1048:1227 (69 bytes, 16 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_BackspaceMessage
 *
 * Removes the last character of the window's message buffer at +0x149, if any.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void ColChild_BackspaceMessage(ColChild *w)
{
    if (STRLEN(w->own_msg) != 0)
        w->own_msg[STRLEN(w->own_msg) - 1] = 0;
}
