/* continent_get: candidate for 1038:b9a8 (157 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The byte at (x, y) of map plane 2: 0 when the index is out of range, otherwise the
 * plane is locked on first use (handle and pointer at DGROUP 0x1c6e / 0x1c7a) and indexed as a
 * pointer, x first then y * width.
 *
 * RENAMED 2026-09-26 from map_plane2_at: the Mac build's continent_get(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller continent_at; caller owner_of; callee MSHLock), the runner-up gets 1, and the arity agrees (2).
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40 */
/* same object: 4-byte `push cs; call ` (rule 47) */

unsigned char continent_get(int x, int y)
{
    unsigned char *p;

    if (!map_index_in_range(x, y))
        return 0;
    if (g_plane2 == 0)
        g_plane2 = (unsigned char *)MSHLock(g_plane2_handle);
    p = g_plane2;
    p += x;
    p += y * g_map_size.w;
    return *p;
}
