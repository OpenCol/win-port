/* elevate: candidate for 1020:031d (353 bytes).  Verdict: RELOCATABLE.
 * symbol: elevate
 * name-source: sibling-build
 *
 * One stroke of the map generator. Plane 3 is cleared and used as SCRATCH: a random interior
 * point is chosen, a brush paints into plane 3, and then every non-zero cell of plane 3 is
 * added into plane 1 and counted at DGROUP:0x4bd4. Plane 1 therefore accumulates how many
 * strokes have touched each square, not merely whether one has.
 *
 * The point is chosen inside a margin -- 7 and 8 in x, 3 and 5 in y -- so a stroke never
 * starts against the edge of the map.
 *
 * THE ARGUMENT PICKS BOTH THE TARGET AND THE BRUSH. Non-zero means land: the point is rejected
 * and re-rolled until it lands where plane 1 is still empty, and `island` runs once,
 * twice at a roll of 7 or more, three times at 8 or more -- so three tenths of strokes are triple
 * and a tenth double. Zero takes the first point it is given and uses §294's diagonal brush
 * when SEG22:0x2 is 2 or more, or `blob` otherwise.
 *
 * The merge walks both planes as `` pointers -- every increment normalized through
 * `__AHSHIFT` -- so it works on a map over 64K squares. `*p++` in the test and a bare `q++`
 * after the body is what the shipped does: plane 3's pointer advances in the condition,
 * plane 1's after the `if`.
 *
 * RENAMED 2026-09-26 from grow_landmass: the Mac build's elevate(short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller build_map; callee buffer_fill), the runner-up gets 1, the arity agrees (1), and the Mac body is 0.80x this one's size (bound pairs: median 1.01, 80% within 0.76-1.83). The margin is small, so the binding also rests on the names describing the same behaviour.
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */

extern void buffer_fill(short handle, char fill_byte);
extern unsigned char buffer_get(short handle, int x, int y);

extern int build_check(int flag);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void island(int x, int y);
extern void blob2(int x, int y);
extern void blob(int x, int y);

void elevate(int pick_land)
{
    int r;
    unsigned char *p;
    unsigned char *q;
    register int x;
    register int y;

    buffer_fill(g_plane3_handle, 0);
    do {
        x = rand_range(1, g_map_size.w - 0x10) + 7;
        y = rand_range(1, g_map_size.h - 8) + 3;
    } while (pick_land != 0 && buffer_get(g_plane1_handle, x, y) != 0);
    if (pick_land != 0) {
        r = rand_range(1, 10);
        island(x, y);
        if (r >= 7)
            island(x, y);
        if (r >= 8)
            island(x, y);
    } else {
        if (g_world_opt[1] >= 2)
            blob2(x, y);
        else
            blob(x, y);
    }
    p = g_plane3;
    q = g_plane1;
    for (y = 0; g_map_size.h > y; y++) {
        for (x = 0; g_map_size.w > x; x++) {
            if (*p++ != 0) {
                (*q)++;
                g_land_count++;
            }
            q++;
        }
    }
    build_check(0);
}
