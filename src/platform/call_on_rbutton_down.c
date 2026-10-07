/* call_on_rbutton_down: candidate for 1070:9cdb (41 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_on_rbutton_down
 *
 * Calls the handler stored at +0x10 of the table with (a, b) when one is set --
 * the invoker paired with the set_on_rbutton_down setters.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

void call_on_rbutton_down(EventTable *table, int x, int y)
{
    if (table->on_rbutton_down != 0)
        ((void (*)(int, int))table->on_rbutton_down)(x, y);
}
