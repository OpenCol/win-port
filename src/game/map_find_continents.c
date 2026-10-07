/* map_find_continents: candidate for 1038:c2a9 (1138 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 1138, not the census 1129. The trailing 9 bytes are the dead second epilogue.
 *
 * NUMBERS THE MAP'S CONNECTED REGIONS -- the continents and the oceans -- for `1020:09ea`, which calls it
 * once the map is generated. It returns a two-bit answer: bit 1 is set if the water ran out of ids and bit
 * 0 if the land did.
 *
 * TWO PASSES, water (1) then land (0). Each pass walks the map from the top down and each row from the
 * right, giving every square of the pass's kind the id of a neighbour above it, or a fresh id when there
 * is none. When two ids meet at one square they are MERGED: the higher id is renumbered to the lower over
 * everything labelled so , and its count is cleared. A fresh id is the first with a zero count, and
 * the first and last rows of the land pass start looking at 0x10 so the poles get their own numbers.
 *
 * Then the pass RENUMBERS what it labelled down to 15 ids: the regions are visited in map order, each new
 * one takes the next free number 1..15 and its count moves there, and its old slot keeps the NEGATIVE of
 * the new number so later squares of the same region find it. Once 15 are used the rest become id 15 and
 * the overflow bit is set. The result is copied into the second plane as it goes.
 *
 * The sizes of the 16 ids are left in the words at SEG27:0x8c.
 *
 * A SHIPPED BUG: the merge writes `counts[lo] += counts[lo]`, doubling the surviving region's count
 * instead of adding the count of the region being absorbed. The absorbed count is then cleared, so it is
 * lost.
 *
 * Spellings: the free-id searches are `while (counts[++id] != 0) ;` -- the increment inside the test,
 * which is what puts the empty `EB 00` entry jump in front of them; the renumbering test is `<= 0` with
 * that arm first and a ternary inside it; the counts are UNSIGNED; and the two copy loops compare
 * `k <= y` and `j <= g_map_size.w` in that order.
 *
 * RENAMED 2026-09-26 from label_map_regions: the Mac build's map_find_continents(void), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller build_map; callee MSAllocateHandle; callee MSDisposeHandle; callee MSHLock; callee MSHUnlock), the runner-up gets 4, and the arity agrees (0). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40 */

extern void buffer_fill(short handle, char fill_byte);
extern int terrain_is_ocean(int x, int y);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern int build_check(int flag);

/* same object: 4-byte `push cs; call ` (rule 47) */

int map_find_continents(void)
{
    int flags;
    int x;
    int y;
    int k;
    int pass;
    int next_id;
    int cur;
    int region;
    int placed;
    int dx;
    int lo;
    int hi;
    int i;
    unsigned char *p;
    unsigned char *q;
    unsigned *counts;
    short h;
    register int j;

    flags = 0;
    counts = 0;
    h = MSAllocateHandle(0x4000L);
    counts = (unsigned *)MSHLock(h);
    buffer_fill(g_plane2_handle, 0);
    for (pass = 1; pass >= 0; pass--) {
        memset_rtl(counts, 0, 0x4000);
        buffer_fill(g_plane3_handle, 0);
        next_id = 0;
        cur = 0;
        placed = 0;
        for (y = 1; g_map_size.h - 1 > y; y++) {
            for (x = g_map_size.w - 2; x >= 1; x--) {
                if (on_map(x, y) == 0 || terrain_is_ocean(x, y) != pass) {
                    placed = 0;
                    cur = 0;
                    continue;
                }
                for (dx = -1; dx <= 1; dx++) {
                    region = *(g_plane3 + (x + dx) + (y - 1) * g_map_size.w);
                    if (region != 0) {
                        lo = region;
                        if (cur != 0 && cur != region) {
                            lo = min_int_1038_e446(region, cur);
                            hi = max_int_1038_e46c(region, cur);
                            counts[lo] += counts[lo];
                            counts[hi] = 0;
                            for (k = 1; k <= y; k++)
                                for (j = 1; j <= g_map_size.w; j++) {
                                    p = g_plane3 + j + k * g_map_size.w;
                                    if (*p == (unsigned char)hi)
                                        *p = (unsigned char)lo;
                                }
                        }
                        cur = region = lo;
                    }
                }
                if (cur == 0) {
                    if (placed == 0) {
                        next_id = 0;
                        if ((y == 1 || g_map_size.h - 2 == y) && pass == 0)
                            next_id = 0x10;
                        while (counts[++next_id] != 0)
                            ;
                        if (next_id >= 0x2000)
                            next_id = 0x1fff;
                    }
                    cur = next_id;
                }
                p = g_plane3 + x + y * g_map_size.w;
                *p = (unsigned char)cur;
                counts[cur]++;
                placed = 1;
            }
            build_check(0);
        }
        p = g_plane3;
        q = g_plane2;
        for (y = 0; y < g_map_size.h; y++) {
            for (x = 0; x < g_map_size.w; x++) {
                if (*p != 0) {
                    if (*p > 0xf) {
                        if (counts[*p] <= 0) {
                            *p = (unsigned char)(counts[*p] > 0 ? counts[*p] : ~counts[*p] + 1);
                        } else {
                            next_id = 0;
                            while (counts[++next_id] != 0)
                                ;
                            if (next_id <= 0xf) {
                                counts[next_id] = counts[*p];
                                counts[*p] = -next_id;
                                *p = (unsigned char)next_id;
                            } else {
                                flags |= 1 << pass;
                                *p = 0xf;
                            }
                        }
                    }
                    *q = *p;
                }
                p++;
                q++;
            }
            build_check(0);
        }
    }
    for (i = 0; i < 0x10; i++)
        g_region_size[i] = counts[i];
    counts = (unsigned *)MSHUnlock(h);
    MSDisposeHandle(h);
    return flags;
}
