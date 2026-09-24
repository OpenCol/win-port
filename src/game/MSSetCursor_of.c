/* MSSetCursor_of -- shipped at 1038:e219.
 *
 * Ported by tools/port.py from matched/game/MSSetCursor_of.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSSetCursor(void *w, col_i16 cur, col_i16 now);

void MSSetCursor_of(GameWinBase *o, col_i16 cur, col_i16 now)
{
    MSSetCursor(o->win, cur, now);
}
