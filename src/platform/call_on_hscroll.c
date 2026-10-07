/* call_on_hscroll: candidate for 1070:9f67 (38 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_on_hscroll
 *
 * Invokes handler slot +76 (0x4c) of the widget layer's handler table with one word
 * argument when a handler is installed.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

void call_on_hscroll(EventTable *table, int pos)
{
    if (table->on_hscroll != 0)
        ((void (*)(int))table->on_hscroll)(pos);
}
