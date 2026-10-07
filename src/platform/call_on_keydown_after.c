/* call_on_keydown_after: candidate for 1070:9dcc (38 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_on_keydown_after
 *
 * Invokes handler slot +40 (0x28) of the widget layer's handler table with one word
 * argument when a handler is installed.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

void call_on_keydown_after(EventTable *table, int key)
{
    if (table->on_keydown_after != 0)
        ((void (*)(int))table->on_keydown_after)(key);
}
