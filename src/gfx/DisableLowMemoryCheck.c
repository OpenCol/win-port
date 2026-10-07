/* DisableLowMemoryCheck: candidate for 1068:bd25 (39 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * `EnableLowMemoryCheck`'s inverse, and its first three instructions verbatim: release the
 * watchdog timer if one is running, then clear the slot. Where the starter goes on to re-arm,
 * this one stops.
 *
 * `ResetTimer` is -called THROUGH A NOP and so is external (rule 47).
 *
 * RENAMED 2026-09-27 from low_memory_watch_stop: the Mac build's DisableLowMemoryCheck, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Releases the low-memory watchdog timer and clears its slot; its two callers, MainWin_CanClose and Opening_dtor, are the Mac routine's two callers; 0 args. The 27x ratio comes from the size estimate running to the next known Mac symbol 1070 bytes on (the Mac body makes no calls), so it is not evidence against.
 */

#include "colonization.h"

void DisableLowMemoryCheck(void)
{
    if (g_lowmem_timer != 0)
        ResetTimer(g_lowmem_timer);
    g_lowmem_timer = 0;
}
