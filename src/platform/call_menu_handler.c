/* call_menu_handler: candidate for 1070:9bd9 (55 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_menu_handler
 *
 * Calls the handler at +0x7c with (a, b) and returns 1; 0 when none is set. The census
 * size (49) stops inside the else branch; the function runs to its dead epilogue at 55.
 *
 * RENAMED 2026-09-26 from call_handler_slot_124_flag: calls the window's +0x7c menu handler with the item
 * menu_find_item located for the command id, returning whether one was installed; wm_command's
 * fallback when no command handler took the id.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

char call_menu_handler(EventWin *win, int item_lo, int item_hi)
{
    if (win->menu_handler != 0) {
        ((void (*)(int, int))win->menu_handler)(item_lo, item_hi);
        return 1;
    } else {
        return 0;
    }
}
