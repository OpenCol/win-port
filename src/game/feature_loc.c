/* feature_loc: candidate for 1038:b76c (123 bytes, 9 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A pointer to (x, y) of map plane 1, locking the plane on first use: x first, then
 * y * width. Four identical functions, one per plane, differ only in their globals.
 *
 * RENAMED 2026-09-26 from map_plane1_ptr: the Mac build's feature_loc(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller create_colony; caller perform_orders_clear; caller perform_orders_road; callee MSHLock), the runner-up gets 2, and the arity agrees (2).
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40 */

unsigned char * feature_loc(int x, int y)
{
    unsigned char *p;

    if (g_plane1 == 0)
        g_plane1 = (unsigned char *)MSHLock(g_plane1_handle);
    p = g_plane1;
    p += x;
    p += y * g_map_size.w;
    return p;
}
