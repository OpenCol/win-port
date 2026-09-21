/* timer_tick_slot -- shipped at 1078:3367.
 *
 * Ported by tools/port.py from matched/platform/timer_tick_slot.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef void ( *TimerFn)(col_i16 slot, col_i16 count);
typedef struct { col_i16 f0; TimerFn fn[17]; col_i16 count[17]; } TimerTable;
extern void timer_free(TimerTable *t, col_i16 slot);

void timer_tick_slot(TimerTable *t, col_i16 i)
{
    if (t->fn[i] != 0 && t->count[i] != 0) {
        if (t->count[i] > 0)
            t->count[i]--;
        t->fn[i](i, t->count[i]);
        if (t->count[i] == 0)
            timer_free(t, i);
    }
}
