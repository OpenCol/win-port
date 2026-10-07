/* call_on_lclick: candidate for 1070:9c89 (41 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_on_lclick
 *
 * Calls the handler stored at +0xc of the table with (a, b) when one is set --
 * the invoker paired with the set_on_lclick setters.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

void call_on_lclick(EventTable *table, int x, int y)
{
    if (table->on_lclick != 0)
        ((void (*)(int, int))table->on_lclick)(x, y);
}
