/* LowMemTimerProc: candidate for 1068:bc53 (112 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The low-memory watchdog's timer callback, armed by `EnableLowMemoryCheck` and fired every
 * ten seconds. When `memory_is_low` says free memory is under the threshold at DGROUP:0x324c
 * (1500 KB) -- or any USER heap is under 15 percent -- it shows the warning, but at most three
 * times: the first is unconditional, the next two only once the deadline at DGROUP:0x3258 has
 * passed. When memory recovers the counter is reset, so the three shots are per episode and
 * not per session.
 *
 * `EndLowMemReport` is the other writer of the pair 0x3254/0x3258: it clears the
 * guard this sets and pushes the deadline out by 0xe10 ticks, one minute at MSGetTicks'
 * sixty ticks a second. So the guard is held for as long as the message box is up, and the spacing is
 * applied on the way out rather than here.
 *
 * The tick comparison goes through EAX (`SHL EAX,0x10 / SHRD EAX,EDX,0x10`), which is rule
 * 48's signed/unsigned conversion between the API's return and the stored deadline.
 *
 * `memory_is_low` is a call; the other two are -called and are stubs here (rule 47).
 *
 * RENAMED 2026-09-27 from low_memory_timer: BEYOND THE PROPOSALS. Mac LowMemTimerProc(short, short), CODE 1 0x21c0, 136 bytes. Its seq is MSLowMemResources, MSGetTicks, PostLowMemReport. bc53 (void), 112 bytes, calls memory_is_low, MSGetTicks and low_memory_warn_show (= PostLowMemReport). Neither has a direct caller; it is the timer callback that EnableLowMemoryCheck / low_memory_watch_start installs. Layout: directly after PostLowMemReport on both. Arity: timer procs differ by platform ((void) against (short, short)).
 */

#include "colonization.h"

#include "classes.h"

/* same object: 4-byte `push cs; call ` */

extern void PostLowMemReport(int res, void *win, int flag);

void LowMemTimerProc(void)
{
    if (memory_is_low(g_lowmem_kb) != 0) {
        if (g_lowmem_busy == 0
            && (g_lowmem_count == 0
                || (g_lowmem_count < 3
                    && (long)MSGetTicks_1068_d953() > g_lowmem_deadline))) {
            g_lowmem_busy = 1;
            g_lowmem_count++;
            PostLowMemReport(g_lowmem_res, (EventWin *)g_lowmem_win, g_lowmem_flag);
        }
    } else {
        g_lowmem_count = 0;
    }
}
