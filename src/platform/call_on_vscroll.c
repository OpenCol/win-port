/* call_on_vscroll: candidate for 1070:9fe6 (38 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_on_vscroll
 *
 * Invokes handler slot +84 (0x54) of the widget layer's handler table with one word
 * argument when a handler is installed.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

void call_on_vscroll(EventTable *table, int pos)
{
    if (table->on_vscroll != 0)
        ((void (*)(int))table->on_vscroll)(pos);
}
