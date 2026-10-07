/* feature_free: candidate for 1038:b903 (42 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Unlocks map plane 1 when it is locked.
 *
 * RENAMED 2026-09-26 from map_plane1_unlock: the Mac build's feature_free(void), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between feature_set and continent_loc, where the Mac gap holds 1 functions and the Windows one 2, so the pairing inside it is by size and arity (the Mac body is 1.33x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

void feature_free(void)
{
    if (g_plane1 != 0)
        g_plane1 = (unsigned char *)MSHUnlock(g_plane1_handle);
}
