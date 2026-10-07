/* MrTimer_Reset: candidate for 1068:9190 (56 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MrTimer_Reset
 * name-source: sibling-build
 *
 * Releases timer slot `slot`: clears the callback pointer and kills the Windows timer whose
 * id is the slot number. The undo of `MrTimer_Set` 139 bytes above it, and it reuses that
 * function's layout exactly -- `short hwnd` then seventeen 4-byte slots from +2, so
 * `&t->slots[slot]` is `t + 2 + slot * 4` and the shipped `BX = t + slot * 4` addresses it
 * as `[bx+2]`.
 *
 * The NULL goes in as two word stores, SEGMENT first: `MOV [bx+4],0` then `MOV [bx+2],0`.
 *
 * RENAMED 2026-09-26 from timer_free: the Mac build's MrTimer::Reset(short): this plus a slot, released and its Windows timer killed. ResetTimer, the free function reporting "MrTimer not initialized in ResetTimer", is its caller on the global timer. The width matches.
 */

#include "colonization.h"

#include "classes.h"
extern void MSResetTimer(short hwnd, int id);

void MrTimer_Reset(MrTimer *timer, register int slot)
{
    timer->slots[slot] = 0;
    MSResetTimer(timer->hwnd, slot);
}
