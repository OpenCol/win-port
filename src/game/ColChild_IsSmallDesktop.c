/* ColChild_IsSmallDesktop: candidate for 1048:069b (69 bytes, 5 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_IsSmallDesktop
 * name-source: sibling-build
 *
 * 1 when the desktop is at most 640 wide, else 0. The window argument its five call
 * sites all push is never read; it is declared so the callers state the truth.
 *
 * RENAMED 2026-09-26 from desktop_fits_640: the Mac build's ColChild::IsSmallDesktop(void): its one parameter is the unread `this`; 1 when the desktop is at most 640 wide. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

int ColChild_IsSmallDesktop(ColChild *w)
{
    RECT rect;

    MSGetDesktopSize(&rect);
    if (rect_width_1048_d69b(&rect) <= 0x280)
        return 1;
    return 0;
}
