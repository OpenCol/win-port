/* timer_alloc -- shipped at 1068:9105.
 *
 * Ported by tools/port.py from matched/gfx/timer_alloc.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { void *fn; } Slot;
typedef struct { col_i16 hwnd; Slot slots[17]; col_i16 args[17]; } Timers;
extern col_i16 MSGetNewTimer(col_i16 hwnd, col_i16 ticks, col_i16 id);

col_i16 timer_alloc(Timers *t, void *fn, col_i16 ticks, col_i16 arg)
{
    col_i16 i;

    for (i = 1; i <= 16; i++) {
        if (t->slots[i].fn == 0)
            break;
    }
    if (i > 16)
        return 0;
    if (MSGetNewTimer(t->hwnd, ticks, i) != 0) {
        t->slots[i].fn = fn;
        t->args[i] = arg;
        return i;
    } else
        return 0;
}
