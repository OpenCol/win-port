/* EnableLowMemoryCheck: candidate for 1068:bcc3 (98 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Arms the low-memory watchdog: remembers the threshold, the resource id of the message, the
 * window to show it over and a flag, clears the counter and the in-progress guard, and starts
 * a repeating timer on `LowMemTimerProc`. Any timer already running is released
 * first, so calling it twice re-arms rather than stacking.
 *
 * `secs` is seconds and the timer wants milliseconds, hence the `* 1000`. `MainWin_Init` and
 * `Opening_ctor` are the callers and both pass (1500, 10, 666, a window, 1) -- so: check
 * every ten seconds, warn below 1.5 MB, and the warning is resource 666.
 *
 * The two calls are both preceded by a NOP, so both callees are external to this
 * translation unit even though they live in this segment (rule 47).
 *
 * RENAMED 2026-09-27 from low_memory_watch_start: BEYOND THE PROPOSALS. Mac EnableLowMemoryCheck(short, short, unsigned short, EventWin&, short), CODE 1 0x2248, 146 bytes. Its callers are MainWin::Init and the Opening ctor. bcc3 (int kb, int secs, int msg_res, void *win, int flag), 98 bytes; its callers are MainWin_Init and Opening_ctor, and its callees are ResetTimer and start_timer. The five arguments match in order and width. Layout: directly after LowMemTimerProc, before DisableLowMemoryCheck (already the Mac name at 1068:bd25), on both.
 */

#include "colonization.h"

#include "classes.h"

extern void LowMemTimerProc(void);

void EnableLowMemoryCheck(int kb, int secs, int msg_res, EventWin *win, int flag)
{
    if (g_lowmem_timer != 0)
        ResetTimer(g_lowmem_timer);
    g_lowmem_kb = kb;
    g_lowmem_res = msg_res;
    g_lowmem_win = win;
    g_lowmem_flag = flag;
    g_lowmem_count = 0;
    g_lowmem_busy = 0;
    g_lowmem_timer = start_timer((void *)LowMemTimerProc, secs * 1000, -1);
}
