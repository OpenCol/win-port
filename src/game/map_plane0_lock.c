/* map_plane0_lock: candidate for 1038:b67b (42 bytes).  Verdict: RELOCATABLE.
 *
 * Locks map plane 0 into its pointer when not already locked.
 */

#include "colonization.h"

void map_plane0_lock(void)
{
    if (g_plane0 == 0)
        g_plane0 = (unsigned char *)MSHLock(g_plane0_handle);
}
