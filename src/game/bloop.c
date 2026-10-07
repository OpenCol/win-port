/* bloop: candidate for 1020:0000 (168 bytes).  Verdict: RELOCATABLE.
 * symbol: bloop
 * name-source: sibling-build
 *
 * The map generator's smallest mark: it sets (x, y) in the scratch plane and, when there is
 * room, (x+1, y) and (x, y+1) as well -- an L, not a square. The fourth corner (x+1, y+1) is
 * never set, so a single call leaves a three-cell wedge and it is the overlap of neighbouring
 * calls that fills a region in.
 *
 * The guard rejects x or y of ZERO as well as anything at or past the map's width and height,
 * so column 0 and row 0 can never be marked; the generator's coastline starts at 1.
 *
 * RENAMED 2026-09-27 from mark_corner: the Mac build's bloop(short, short): the square-and-two-neighbours land brush that blob (walk_diagonal) calls once per step, then build_check -- the Mac blob's sequence.
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */
extern void buffer_put(short handle, int x, int y, char v);
extern int build_check(int flag);

void bloop(register int x, register int y)
{
    if (x != 0 && y != 0 && g_map_size.w > x && g_map_size.h > y) {
        buffer_put(g_plane3_handle, x, y, 1);
        if (g_map_size.w - 1 > x)
            buffer_put(g_plane3_handle, x + 1, y, 1);
        if (g_map_size.h - 1 > y)
            buffer_put(g_plane3_handle, x, y + 1, 1);
    }
    build_check(0);
}
