/* timer_free -- shipped at 1068:9190.
 *
 * Ported by tools/port.py from matched/gfx/timer_free.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { void *fn; } Slot;
typedef struct { col_i16 hwnd; Slot slots[17]; col_i16 args[17]; } Timers;
extern void kill_timer(col_i16 hwnd, col_i16 id);

void timer_free(Timers *t, register col_i16 slot)
{
    t->slots[slot].fn = 0;
    kill_timer(t->hwnd, slot);
}
