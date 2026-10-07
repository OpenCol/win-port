/* call_on_setfocus: candidate for 1070:9e3e (32 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_on_setfocus
 *
 * Invokes handler slot +60 (0x3c) of the widget layer's handler table, without
 * arguments, when a handler is installed.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

void call_on_setfocus(EventTable *handlers)
{
    if (handlers->on_setfocus != 0)
        ((void (*)(void))handlers->on_setfocus)();
}
