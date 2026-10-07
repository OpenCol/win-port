/* MSCloseSound: candidate for 1078:0559 (24 bytes).  Verdict: RELOCATABLE.
 *
 * sndPlaySound(NULL, SND_ASYNC) -- the documented way to STOP whatever is playing. Same
 * import and same flag as MSPlaySound; only the null name distinguishes them.
 */

#include "colonization.h"

void MSCloseSound(void)
{
    sndPlaySound(0, 1);
}
