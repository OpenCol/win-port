/* win_move_to_of -- shipped at 1068:df2a.
 *
 * Ported by tools/port.py from matched/gfx/win_move_to_of.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void win_move_to_1078_1ff8(void *w, col_i16 x, col_i16 y);

void win_move_to_of(GameWinBase *o, col_i16 x, col_i16 y)
{
    win_move_to_1078_1ff8(o->win, x, y);
}
