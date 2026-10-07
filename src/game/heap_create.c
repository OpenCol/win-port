/* heap_create: candidate for 1040:6a8a (143 bytes, 3 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Allocates a memory block descriptor's handle of `size` bytes, tagging it with `kind`.
 * On success the descriptor is initialised (flag 1, pointer 0, two size fields) and 0 is
 * returned; on failure the error path runs with "Heap Request Failed" and the kind, and 1
 * is returned. The routine it hands them to, `memblock_hook` (1040:a9f4), is fourteen bytes
 * and EMPTY in the shipped build -- the message is composed and dropped. It is named for
 * what it does, not for what its arguments say it was meant to do. `kind` is an int stored into a char field: the error path sign-extends a word.
 *
 * RENAMED 2026-09-26 from memblock_alloc: the Mac build's heap_create(Heap*, int, unsigned short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller set_heap; callee MSAllocateHandle), the runner-up gets 1, and the arity agrees (3). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

int heap_create(MemBlock *heap, int kind, unsigned size)
{
    unsigned n = size;
    int err = 1;

    heap->kind = kind;
    heap->handle = MSAllocateHandle(n);
    if (heap->handle == 0)
        memblock_hook(&g_error_log, "Heap Request Failed", (long)kind);
    else {
        heap->owns_handle = 1;
        heap->used = 0;
        heap->avail = n;
        heap->size = n;
        heap->ptr = 0;
        err = 0;
    }
    return err;
}
