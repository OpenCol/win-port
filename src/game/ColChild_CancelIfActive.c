/* ColChild_CancelIfActive: candidate for 1048:128e (56 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_CancelIfActive
 * name-source: descriptive
 *
 * Cancels the colony child's message only if one is showing, and reports whether it did.
 * +0x75e is the flag `ColChild_ActivateMessage` sets and `ColChild_CancelMessage` clears.
 *
 * The early exit is a `goto`, not an `if` around the body: the shipped code inverts the
 * test and jumps over a `JMP` (rule 12).
 *
 * size-source: 56, not the census 51. The last 5 bytes are a second, unreachable epilogue
 * -- Borland emits one per `return` path and only one of them can be reached.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ColChild_CancelMessage(ColChild *child, int a);
int ColChild_CancelIfActive(ColChild *w)
{
    register int cancelled = 0;

    if (w->active == 0)
        goto done;
    ColChild_CancelMessage(w, 0);
    cancelled = 1;
done:
    return cancelled;
}
