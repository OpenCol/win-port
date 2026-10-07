/* win_is_visible: candidate for 1078:1d24 (60 bytes).  Verdict: RELOCATABLE.
 *
 * IsWindowVisible on the window's handle (+2) as 0/1; 0 for a null window.

 * The stated size was 56 and the function is 60: `tools/re_agent/check_sizes.py`
 * found the extra bytes decoding as an unreachable epilogue, and the reconstruction
 * already grades over the whole extent. Only the number moved.
 */

#include "colonization.h"

#include "records.h"

int win_is_visible(MSWin *win)
{
    if (win != 0) {
        if (IsWindowVisible(win->hwnd) != 0)
            return 1;
        else
            return 0;
    } else {
        return 0;
    }
}
