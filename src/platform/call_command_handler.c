/* call_command_handler: candidate for 1070:9ba2 (49 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_command_handler
 *
 * Calls the handler at +0x80 with the long argument (pushed as one dword) and returns 1; 0 when none is set.
 *
 * RENAMED 2026-09-26 from call_handler_slot_128_flag: calls the window's +0x80 command handler -- the slot
 * set_command_handler fills -- with the WM_COMMAND id, returning whether one was installed;
 * wm_command falls back to the menu only when it returns 0.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

char call_command_handler(EventWin *table, long command)
{
    if (table->command_handler != 0) {
        ((void (*)(long))table->command_handler)(command);
        return 1;
    } else {
        return 0;
    }
}
