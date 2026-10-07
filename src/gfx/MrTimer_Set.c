/* MrTimer_Set: candidate for 1068:9105 (132 bytes, 1 caller).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MrTimer_Set
 * name-source: sibling-build
 *
 * Finds the first free of 16 timer slots (1..16), starts a timer on the object's window
 * with the slot as its id, records the callback and an argument, and returns the slot;
 * 0 when no slot or the timer fails.
 *
 * RENAMED 2026-09-26 from timer_alloc: the Mac build's MrTimer::Set(void(*)(short, short)*, short, short): this plus the callback, the interval and the repeat count, in that order. The width matches.
 */

#include "colonization.h"

#include "classes.h"

int MrTimer_Set(MrTimer *timer, void *fn, int ticks, int count)
{
    int i;

    for (i = 1; i <= 16; i++) {
        if (timer->slots[i] == 0)
            break;
    }
    if (i > 16)
        return 0;
    if (MSGetNewTimer(timer->hwnd, ticks, i) != 0) {
        timer->slots[i] = (TimerFn)fn;
        timer->counts[i] = count;
        return i;
    } else
        return 0;
}
