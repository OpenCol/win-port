/* MainWin_CanClose: candidate for 1048:adf3 (56 bytes).  Verdict: RELOCATABLE.
 *
 * Asks "DOS"; answer 2 runs DisableLowMemoryCheck, sets the quit flag at DGROUP:0x4b04
 * and returns 1, anything else 0.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void DisableLowMemoryCheck(void);

int MainWin_CanClose(void)
{
    if (ask_text_resource("DOS") == 2) {
        DisableLowMemoryCheck();
        g_quit_flag = 1;
        return 1;
    } else {
        return 0;
    }
}
