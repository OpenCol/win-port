/* call_on_ldblclk: candidate for 1070:9d7d (41 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_on_ldblclk
 *
 * Calls the handler stored at +0x1c of the table with (a, b) when one is set --
 * the invoker paired with the set_on_ldblclk setters.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

void call_on_ldblclk(EventTable *table, int x, int y)
{
    if (table->on_ldblclk != 0)
        ((void (*)(int, int))table->on_ldblclk)(x, y);
}
