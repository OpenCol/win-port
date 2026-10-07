/* map_plane2_lock: candidate for 1038:babf (42 bytes).  Verdict: RELOCATABLE.
 *
 * Locks map plane 2 into its pointer when not already locked.
 */

#include "colonization.h"

void map_plane2_lock(void)
{
    if (g_plane2 == 0)
        g_plane2 = (unsigned char *)MSHLock(g_plane2_handle);
}
