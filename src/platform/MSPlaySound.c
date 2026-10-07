/* MSPlaySound: candidate for 1078:053e (27 bytes).  Verdict: RELOCATABLE.
 *
 * sndPlaySound(name, SND_ASYNC) -- fire and forget, the call returns before the sound does.
 */

#include "colonization.h"

void MSPlaySound(const char *name)
{
    sndPlaySound(name, 1);
}
