/* win_set_cursor_1060_6275 -- shipped at 1060:6275.
 *
 * Ported by tools/port.py from matched/game/win_set_cursor_1060_6275.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void win_set_cursor_1078_28e4(void *w, col_i16 v, col_i16 a);

void win_set_cursor_1060_6275(GameWinBase *o, col_i16 *p, col_i16 a)
{
    win_set_cursor_1078_28e4(o->win, *p, a);
}
