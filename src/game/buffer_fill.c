/* buffer_fill: candidate for 1040:3ea6 (85 bytes, 7 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * memsets the whole block behind a memory handle to c: lock, MSBlockSize, memset, unlock.
 *
 * RENAMED 2026-09-26 from handle_fill_bytes: the Mac build's buffer_fill(char**, unsigned char), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller build_map; callee MSBlockSize; callee MSHLock; callee MSHUnlock), the runner-up gets 3, and the arity agrees (2). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

void buffer_fill(short handle, char fill_byte)
{
    short h = handle;
    void *p;

    p = MSHLock(h);
    memset_rtl(p, (unsigned char)fill_byte, MSBlockSize(h));
    p = MSHUnlock(h);
}
