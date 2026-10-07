/* spec_nibble_tier: candidate for 1018:4d2b (70 bytes).  Verdict: RELOCATABLE.
 * symbol: spec_nibble_tier
 *
 * Buckets a colonist's nibble value (get_progress) into 0..3 at the
 * thresholds 4, 8 and 15.  The helper is -called and defined in-file
 * as a stub.
 */

#include "colonization.h"

extern int g_stub_a;

extern int get_progress(int colonist);

int spec_nibble_tier(int colonist)
{
    register int r;
    register int progress;

    progress = get_progress(colonist);
    if (progress >= 15)
        r = 3;
    else if (progress >= 8)
        r = 2;
    else if (progress >= 4)
        r = 1;
    else
        r = 0;
    return r;
}
