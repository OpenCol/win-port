/* PORT: this file's own declarations of get_win_1060_ab6e, wait_mouse_button removed: the header declares them from the definition (W1 step 3). */
/* Screen_RButtonPressed: candidate for 1060:a772 (80 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -r-
 * symbol: Screen_RButtonPressed
 *
 * Waits on the RIGHT mouse button -- wait_mouse_button with left = 0 -- on the object's own window when the caller asks for it and on a null window (the whole queue) otherwise.
 *
 * RENAMED 2026-09-26 from msg_box_1060_a772: its twin at the next address passes up = 0, which makes wait_mouse_button watch the RIGHT button (0x205/0xa5). The Mac Screen class has only LButtonPressed, so this Windows-only sibling is named by analogy -- descriptively, not from the Mac table.
 */

#include "colonization.h"

#include "classes.h"
#include "records.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
void Screen_RButtonPressed(void *o, char own, int remove, int wait)
{
    if (own) {
        wait_mouse_button((MSWin *)(get_win_1060_ab6e((Screen *)o)), 0, remove, wait);
        return;
    } else {
        wait_mouse_button(0, 0, remove, wait);
        return;
    }
}
