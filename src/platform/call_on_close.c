/* call_on_close: candidate for 1070:9efb (44 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_on_close
 *
 * Invokes handler slot +48 (0x30) of the widget layer's handler table and returns its
 * result; 0 when no handler is installed.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

int call_on_close(EventTable *table)
{
    if (table->on_close != 0)
        return ((int (*)(void))table->on_close)();
    else
        return 0;
}
