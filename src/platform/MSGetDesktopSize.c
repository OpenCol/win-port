/* MSGetDesktopSize: candidate for 1078:2566 (45 bytes).  Verdict: RELOCATABLE.
 *
 * SetRect(r, 0, 0, SM_CXSCREEN, SM_CYSCREEN).
 */

#include "colonization.h"

#include "w16types.h"

void MSGetDesktopSize(void *rect)
{
    SetRect((RECT *)rect, 0, 0, GetSystemMetrics(0), GetSystemMetrics(1));
}
