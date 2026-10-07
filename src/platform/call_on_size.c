/* call_on_size: candidate for 1070:9e6c (38 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_on_size
 *
 * Invokes handler slot +64 (0x40) of the widget layer's handler table with one word
 * argument when a handler is installed.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

void call_on_size(EventTable *table, int size_type)
{
    if (table->on_size != 0)
        ((void (*)(int))table->on_size)(size_type);
}
