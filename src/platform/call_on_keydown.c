/* call_on_keydown: candidate for 1070:9da6 (38 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_on_keydown
 *
 * Invokes handler slot +32 (0x20) of the widget layer's handler table with one word
 * argument when a handler is installed.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

void call_on_keydown(EventTable *table, int key)
{
    if (table->on_keydown != 0)
        ((void (*)(int))table->on_keydown)(key);
}
