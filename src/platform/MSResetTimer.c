/* MSResetTimer: candidate for 1078:194f (25 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * `KillTimer(hwnd, id)` and nothing else -- `enable_window`'s twin.
 *
 * RENAMED 2026-09-27 from kill_timer: the Mac build's MSResetTimer, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: KillTimer(hwnd, id), the platform routine right after MSGetNewTimer (1078:190c+67 = 1078:194f); its only caller is MrTimer_Reset, the Mac MSResetTimer's only caller is MrTimer::Reset; 2 args; the Mac body (ratio 2.96) does Time Manager work that has no Windows counterpart.
 */

#include "colonization.h"

void MSResetTimer(unsigned hwnd, int id)
{
    KillTimer(hwnd, id);
}
