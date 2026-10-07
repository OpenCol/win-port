/* MSShowCursor: candidate for 1078:291a (21 bytes).  Verdict: RELOCATABLE.
 *
 * ShowCursor(TRUE). The sibling MSHideCursor right after it has to SPIN, because Windows
 * keeps a show COUNT and one call only moves it by one.
 */

#include "colonization.h"

void MSShowCursor(void)
{
    ShowCursor(1);
}
