/* call_on_rbutton_up: candidate for 1070:9d2d (41 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_on_rbutton_up
 *
 * Calls the handler stored at +0x14 of the table with (a, b) when one is set --
 * the invoker paired with the set_on_rbutton_up setters.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

void call_on_rbutton_up(EventTable *table, int x, int y)
{
    if (table->on_rbutton_up != 0)
        ((void (*)(int, int))table->on_rbutton_up)(x, y);
}
