/* map_loc: candidate for 1038:b600 (123 bytes, 9 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A pointer to (x, y) of map plane 0, locking the plane on first use: x first, then
 * y * width. Four identical functions, one per plane, differ only in their globals.
 *
 * RENAMED 2026-09-26 from map_plane0_ptr: the Mac build's map_loc(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller perform_orders_build; caller perform_orders_clear; callee MSHLock), the runner-up gets 1, and the arity agrees (2).
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40 */

unsigned char * map_loc(int x, int y)
{
    unsigned char *p;

    if (g_plane0 == 0)
        g_plane0 = (unsigned char *)MSHLock(g_plane0_handle);
    p = g_plane0;
    p += x;
    p += y * g_map_size.w;
    return p;
}
