/* map_get: candidate for 1038:b6cf (157 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The byte at (x, y) of map plane 0: 0 when the index is out of range, otherwise the
 * plane is locked on first use (handle and pointer at DGROUP 0x1c6a / 0x1c72) and indexed as a
 * pointer, x first then y * width.
 *
 * RENAMED 2026-09-26 from map_plane0_at: the Mac build's map_get(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (caller Stats; caller indian_move; caller label_map; caller move_unit; caller plan_move), the runner-up gets 4, and the arity agrees (2).
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40 */
/* same object: 4-byte `push cs; call ` (rule 47) */

unsigned char map_get(int x, int y)
{
    unsigned char *p;

    if (!map_index_in_range(x, y))
        return 0;
    if (g_plane0 == 0)
        g_plane0 = (unsigned char *)MSHLock(g_plane0_handle);
    p = g_plane0;
    p += x;
    p += y * g_map_size.w;
    return *p;
}
