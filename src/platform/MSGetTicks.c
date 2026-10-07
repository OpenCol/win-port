/* MSGetTicks: candidate for 1078:181a (48 bytes).  Verdict: RELOCATABLE.
 *
 * GetTickCount() * 6 / 100: milliseconds to 60ths of a second.
 */

#include "colonization.h"

long MSGetTicks(void)
{
    return GetTickCount() * 6L / 100;
}
