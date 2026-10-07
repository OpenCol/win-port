/* continent_free: candidate for 1038:bae9 (42 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Unlocks map plane 2 when it is locked.
 *
 * RENAMED 2026-09-26 from map_plane2_unlock: the Mac build's continent_free(void), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between continent_at and owner_of, where the Mac gap holds 1 functions and the Windows one 3, so the pairing inside it is by size and arity (the Mac body is 1.38x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

void continent_free(void)
{
    if (g_plane2 != 0)
        g_plane2 = (unsigned char *)MSHUnlock(g_plane2_handle);
}
