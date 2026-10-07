/* map_plane1_lock: candidate for 1038:b8d9 (42 bytes).  Verdict: RELOCATABLE.
 *
 * Locks map plane 1 into its pointer when not already locked.
 */

#include "colonization.h"

void map_plane1_lock(void)
{
    if (g_plane1 == 0)
        g_plane1 = (unsigned char *)MSHLock(g_plane1_handle);
}
