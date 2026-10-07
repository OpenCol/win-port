/* call_on_commnotify: candidate for 1070:9f27 (38 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_on_commnotify
 *
 * Invokes handler slot +72 (0x48) of the widget layer's handler table with one word
 * argument when a handler is installed.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

void call_on_commnotify(EventTable *table, int notify_status)
{
    if (table->on_commnotify != 0)
        ((void (*)(int))table->on_commnotify)(notify_status);
}
