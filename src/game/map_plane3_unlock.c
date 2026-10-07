/* map_plane3_unlock: candidate for 1038:bd8d (42 bytes).  Verdict: RELOCATABLE.
 *
 * Unlocks map plane 3 when it is locked.
 */

#include "colonization.h"

void map_plane3_unlock(void)
{
    if (g_plane3 != 0)
        g_plane3 = (unsigned char *)MSHUnlock(g_plane3_handle);
}
