/* map_free: candidate for 1038:b6a5 (42 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Unlocks map plane 0 when it is locked.
 *
 * RENAMED 2026-09-26 from map_plane0_unlock: the Mac build's map_free(void), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between map_loc and map_get, where the Mac gap holds 1 functions and the Windows one 2, so the pairing inside it is by size and arity (the Mac body is 1.24x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

void map_free(void)
{
    if (g_plane0 != 0)
        g_plane0 = (unsigned char *)MSHUnlock(g_plane0_handle);
}
