/* call_on_vthumb: candidate for 1070:a00c (38 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_on_vthumb
 *
 * Invokes handler slot +88 (0x58) of the widget layer's handler table with one word
 * argument when a handler is installed.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

void call_on_vthumb(EventTable *table, int pos)
{
    if (table->on_vthumb != 0)
        ((void (*)(int))table->on_vthumb)(pos);
}
