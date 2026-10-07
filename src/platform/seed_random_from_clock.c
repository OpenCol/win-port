/* seed_random_from_clock: candidate for 1070:dd1d (34 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `srand(time(NULL))`, called once at the end of startup -- after the MSMMWindow is
 * created and WinG.dll is loaded -- and from nowhere else.
 */

#include "colonization.h"

void seed_random_from_clock(void)
{
    srand_rtl((unsigned)time_rtl(0L));
}
