/* site_loc: candidate for 1038:bc4b (123 bytes, 8 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A pointer to (x, y) of map plane 3, locking the plane on first use: x first, then
 * y * width. Four identical functions, one per plane, differ only in their globals.
 *
 * RENAMED 2026-09-26 from map_plane3_ptr: the Mac build's site_loc(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller conquest; caller coronado_colony; caller explore_map_square; callee MSHLock), the runner-up gets 2, and the arity agrees (2).
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40 */

unsigned char * site_loc(int x, int y)
{
    unsigned char *p;

    if (g_plane3 == 0)
        g_plane3 = (unsigned char *)MSHLock(g_plane3_handle);
    p = g_plane3;
    p += x;
    p += y * g_map_size.w;
    return p;
}
