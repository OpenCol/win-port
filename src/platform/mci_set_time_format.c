/* mci_set_time_format: candidate for 1078:2f32 (46 bytes).  Verdict: EXACT.
 *
 * Stores 0x20000 (frames) or 0x10000 (milliseconds) in the long at +8 of
 * the record by the flag.
 */

#include "colonization.h"

void mci_set_time_format(MciRec *r, char frames)
{
    if (frames != 0)
        r->fmt = 0x20000L;
    else
        r->fmt = 0x10000L;
}
