/* continent_loc: candidate for 1038:b92d (123 bytes, 5 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A pointer to (x, y) of map plane 2, locking the plane on first use: x first, then
 * y * width. Four identical functions, one per plane, differ only in their globals.
 *
 * RENAMED 2026-09-26 from map_plane2_ptr: the Mac build's continent_loc(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller conquest; caller owner_set; callee MSHLock), the runner-up gets 1, and the arity agrees (2).
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40 */

unsigned char * continent_loc(int x, int y)
{
    unsigned char *p;

    if (g_plane2 == 0)
        g_plane2 = (unsigned char *)MSHLock(g_plane2_handle);
    p = g_plane2;
    p += x;
    p += y * g_map_size.w;
    return p;
}
