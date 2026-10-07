/* call_on_restore: candidate for 1070:9edb (32 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_on_restore
 *
 * Invokes handler slot +56 (0x38) of the widget layer's handler table, without
 * arguments, when a handler is installed.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

void call_on_restore(EventTable *table)
{
    if (table->on_restore != 0)
        ((void (*)(void))table->on_restore)();
}
