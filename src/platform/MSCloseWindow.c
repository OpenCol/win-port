/* PORT: this file's own declarations of ctl_hwnd_1070_9681, ctl_kind_1070_9669 removed: the header declares them from the definition (W1 step 3). */
/* MSCloseWindow: candidate for 1070:088b (90 bytes).  Verdict: RELOCATABLE.
 *
 * Unless the window's kind (ctl_kind_1070_9669) is 3, destroys its Windows handle
 * (ctl_hwnd_1070_9681) when it still is a window. The pointer goes through a local copy and the
 * outer `if` has an empty `else`.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */

void MSCloseWindow(void *win)
{
    void *w = win;

    if (ctl_kind_1070_9669((DlgControl *)w) != 3) {
        if (IsWindow(ctl_hwnd_1070_9681((DlgControl *)w)))
            DestroyWindow(ctl_hwnd_1070_9681((DlgControl *)w));
    } else
        ;
}
