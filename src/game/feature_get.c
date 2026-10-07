/* feature_get: candidate for 1038:b7e7 (147 bytes, 59 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The byte at (x, y) of map plane 1: 0 when y is off the 72-row map, otherwise the plane
 * is locked on first use (handle and pointer are globals) and indexed as a 
 * pointer, x first then y * width.
 *
 * RENAMED 2026-09-26 from map_plane1_at: the Mac build's feature_get(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (caller Stats; caller colony_plan; caller explore_around_square; caller indian_move; caller label_map), the runner-up gets 6, and the arity agrees (2).
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40 */

unsigned char feature_get(int x, int y)
{
    int yy = y;
    unsigned char *p;

    if (yy >= 0x48 || yy < 0)
        return 0;
    if (g_plane1 == 0)
        g_plane1 = (unsigned char *)MSHLock(g_plane1_handle);
    p = g_plane1;
    p += x;
    p += yy * g_map_size.w;
    return *p;
}
