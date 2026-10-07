/* call_on_char: candidate for 1070:9df2 (38 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_on_char
 *
 * Invokes handler slot +44 (0x2c) of the widget layer's handler table with one word
 * argument when a handler is installed.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

void call_on_char(EventTable *table, int ch)
{
    if (table->on_char != 0)
        ((void (*)(int))table->on_char)(ch);
}
