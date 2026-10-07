/* call_on_move: candidate for 1070:9e92 (41 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_on_move
 *
 * Calls the handler stored at +0x44 of the table with (a, b) when one is set --
 * the invoker paired with the set_on_move setters.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

void call_on_move(EventTable *t, int x, int y)
{
    if (t->on_move != 0)
        ((void (*)(int, int))t->on_move)(x, y);
}
