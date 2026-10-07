/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 21 of the imported file. */
/* PORT: this file's own declarations of heap_release removed: the header declares it from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 20 of the imported file. */
/* heap_destroy: candidate for 1040:6c03 (79 bytes, 3 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Releases a memory-block descriptor: unlock and dispose when its flag is set, then
 * clear the used count (+8) and both sizes (+0xa, +0xc); the pointer at +4 is cleared only by heap_release.
 *
 * RENAMED 2026-09-26 from memblock_free: the Mac build's heap_destroy(Heap*), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller set_heap; callee MSDisposeHandle), the runner-up gets 1, and the arity agrees (1). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
/* same object: 4-byte `push cs; call ` (rule 47) */

void heap_destroy(MemBlock *block)
{
    if (block->owns_handle != 0) {
        heap_release((Resource *)((MemBlock *)((Resource *)block)));
        MSDisposeHandle(block->handle);
    }
    block->used = 0;
    block->avail = 0;
    block->size = 0;
}
