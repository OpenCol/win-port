/* spin_over_count: candidate for 1078:1e2c (32 bytes).  Verdict: RELOCATABLE.
 *
 * Walks the table at DGROUP:0x5728 from 0 to the count at DGROUP:0x3c80 and
 * does nothing with it: the body the loop had is gone, the loop is not.
 */

#include "colonization.h"

void spin_over_count(void)
{
    register int i;

    if (g_ms_window_count != 0) {
        for (i = 0; i < g_ms_window_count; i++)
            ;
    }
}
