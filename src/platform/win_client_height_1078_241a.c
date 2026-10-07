/* win_client_height_1078_241a: candidate for 1078:241a (63 bytes).  Verdict: RELOCATABLE.
 *
 * The client bottom of the window (+2); 0 for a null window.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

int win_client_height_1078_241a(MSWin *win)
{
    RECT r;

    if (win == 0)
        return 0;
    GetClientRect(win->hwnd, &r);
    return r.bottom;
}

