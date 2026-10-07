/* call_on_minimize: candidate for 1070:9ebb (32 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_on_minimize
 *
 * Invokes handler slot +52 (0x34) of the widget layer's handler table, without
 * arguments, when a handler is installed.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

void call_on_minimize(EventTable *table)
{
    if (table->on_minimize != 0)
        ((void (*)(void))table->on_minimize)();
}
