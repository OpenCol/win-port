/* PORT: this file's own declarations of MrTimer_Reset removed: the header declares it from the definition (W1 step 3). */
/* timer_tick_slot: candidate for 1078:3367 (146 bytes).  Verdict: RELOCATABLE.
 *
 * One tick of timer slot i of the MrTimer table: with a handler set and a
 * non-zero count, the count comes down (never below 0), the handler runs
 * with the slot and the count, and a count that reached 0 hands the slot
 * to MrTimer_Reset.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void timer_tick_slot(MrTimer *timer, int i)
{
    if (timer->slots[i] != 0 && timer->counts[i] != 0) {
        if (timer->counts[i] > 0)
            timer->counts[i]--;
        timer->slots[i](i, timer->counts[i]);
        if (timer->counts[i] == 0)
            MrTimer_Reset(timer, i);
    }
}
