/* win_client_width_1078_23db: candidate for 1078:23db (63 bytes).  Verdict: RELOCATABLE.
 *
 * The client right of the window (+2); 0 for a null window.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

int win_client_width_1078_23db(MSWin *win)
{
    RECT r;

    if (win == 0)
        return 0;
    GetClientRect(win->hwnd, &r);
    return r.right;
}

