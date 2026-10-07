/* heap_release: candidate for 1040:6bcf (52 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Unlocks a loaded resource: when the pointer at +4 is set, MSHUnlock
 * on the handle at +2 and the pointer cleared.
 *
 * RENAMED 2026-09-26 from resource_unlock: the Mac build's heap_release(Heap*), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller heap_destroy; callee MSHUnlock), the runner-up gets 1, the arity agrees (1), and the Mac body is 1.19x this one's size (bound pairs: median 1.01, 80% within 0.76-1.83). The margin is small, so the binding also rests on the names describing the same behaviour.
 */

#include "colonization.h"

#include "records.h"

void heap_release(Resource *r)
{
    if (r->data != 0) {
        MSHUnlock(r->handle);
        r->data = 0;
    }
}
