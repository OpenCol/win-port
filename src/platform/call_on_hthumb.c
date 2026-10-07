/* call_on_hthumb: candidate for 1070:9f8d (38 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_on_hthumb
 *
 * Invokes handler slot +80 (0x50) of the widget layer's handler table with one word
 * argument when a handler is installed.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

void call_on_hthumb(EventTable *handlers, int pos)
{
    if (handlers->on_hthumb != 0)
        ((void (*)(int))handlers->on_hthumb)(pos);
}
