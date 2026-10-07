/* rect_contains: candidate for 1068:62b8 (70 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: rect_contains
 *
 * Whether (x, y) is inside the rectangle at +8 of the record.  Written as the
 * NEGATIVE test with an explicit `else`: an `||` chain whose last operand
 * jumps over the body, so `return 0` sits physically first.  Dropping the
 * `else` and falling through to `return 1` compiles differently -- the true
 * arm moves ahead of the false one -- which is the whole difference between
 * this and the version that did not match.
 *
 * RENAMED 2026-09-26 from rect_contains_1068_62b8: the address suffix only told it apart from 1048:174f, which is now InRect -- and was never its twin: this test is half-open (right and bottom excluded) where InRect's is closed.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

char rect_contains(GRPort *port, int x, int y)
{
    if (port->rect.left > x || port->rect.right <= x || port->rect.top > y || port->rect.bottom <= y)
        return 0;
    else
        return 1;
}
