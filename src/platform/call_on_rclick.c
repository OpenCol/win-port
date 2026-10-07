/* call_on_rclick: candidate for 1070:9d04 (41 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_on_rclick
 *
 * Calls the handler stored at +0x18 of the table with (a, b) when one is set --
 * the invoker paired with the set_on_rclick setters.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

void call_on_rclick(EventTable *table, int x, int y)
{
    if (table->on_rclick != 0)
        ((void (*)(int, int))table->on_rclick)(x, y);
}
