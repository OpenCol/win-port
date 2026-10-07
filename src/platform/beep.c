/* beep: candidate for 1078:057f (21 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MessageBeep(-1)` and nothing else -- the standard beep, which is what -1 selects.
 */

#include "colonization.h"

void beep(void)
{
    MessageBeep(-1);
}
