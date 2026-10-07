/* create_timer_daemon_window: candidate for 1078:1885 (106 bytes).  Verdict: RELOCATABLE.
 *
 * Creates the hidden "MSMrTimerClass" window (default size, no parent), reports
 * "Error: Cannot create timer daemon" on failure, stores obj in its window long 0 and returns the handle.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

unsigned create_timer_daemon_window(void *obj)
{
    unsigned h = CreateWindow("MSMrTimerClass", "MrTimer", 0xcf0000L, 0x8000, 0x8000, 0x8000, 0x8000, 0, 0, g_hinstance, 0);

    if (h == 0)
        stub_reporter(&g_error_log, "Error: Cannot create timer daemon");
    SetWindowLong(h, 0, (long)obj);
    return h;
}
