/* call_on_lbutton_up: candidate for 1070:9cb2 (41 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_on_lbutton_up
 *
 * Calls the handler stored at +0x8 of the table with (a, b) when one is set --
 * the invoker paired with the set_on_lbutton_up setters.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

void call_on_lbutton_up(EventTable *table, int x, int y)
{
    if (table->on_lbutton_up != 0)
        ((void (*)(int, int))table->on_lbutton_up)(x, y);
}
