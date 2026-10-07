/* call_on_keyup: candidate for 1070:9e18 (38 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_on_keyup
 *
 * Invokes handler slot +36 (0x24) of the widget layer's handler table with one word
 * argument when a handler is installed.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

void call_on_keyup(EventTable *table, int key)
{
    if (table->on_keyup != 0)
        ((void (*)(int))table->on_keyup)(key);
}
