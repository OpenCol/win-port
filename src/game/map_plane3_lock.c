/* map_plane3_lock: candidate for 1038:bd63 (42 bytes).  Verdict: RELOCATABLE.
 *
 * Locks map plane 3 into its pointer when not already locked.
 */

#include "colonization.h"

void map_plane3_lock(void)
{
    if (g_plane3 == 0)
        g_plane3 = (unsigned char *)MSHLock(g_plane3_handle);
}
