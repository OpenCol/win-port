/* heap_lock: candidate for 1040:6b68 (103 bytes, 1 caller).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Locks a memory-block descriptor's handle into its pointer when not already locked,
 * reporting "Heap Lock Failed" with the kind when the lock returns NULL. Always returns 0:
 * the error flag is set to 1 and then unconditionally cleared, as shipped.
 *
 * RENAMED 2026-09-26 from memblock_lock: the Mac build's heap_lock(Heap*), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller heap_get; callee MSHLock), the runner-up gets 1, and the arity agrees (1). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

int heap_lock(MemBlock *block)
{
    int err = 1;

    if (block->ptr == 0) {
        block->ptr = MSHLock(block->handle);
        if (block->ptr == 0)
            memblock_hook(&g_error_log, "Memory Lock Error", (long)block->kind);
    }
    err = 0;
    return err;
}
