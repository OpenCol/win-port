/* buffer_get: candidate for 1040:3efb (144 bytes, 28 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The byte at (x, y) of a map plane given by its memory handle, or 0 off the 72-row map:
 * lock, index as a pointer by width * y then x, read, unlock.
 *
 * RENAMED 2026-09-26 from map_handle_get: the Mac build's buffer_get(char**, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller build_map; caller elevate; callee MSHLock; callee MSHUnlock), the runner-up gets 3, the arity agrees (3), and the Mac body is 0.72x this one's size. The margin is small, so the binding also rests on the body doing what the Mac name says.
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */

unsigned char buffer_get(short handle, int x, int y)
{
    short h = handle;
    int yy = y;
    unsigned char *p;
    unsigned char v;

    if (yy >= 0x48 || yy < 0)
        return 0;
    else {
        p = (unsigned char *)MSHLock(h);
        p = p + g_map_size.w * yy + x;
        v = *p;
        p = (unsigned char *)MSHUnlock(h);
    }
    return v;
}
