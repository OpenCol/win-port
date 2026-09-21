/* win_move_to_1048_d7e4 -- shipped at 1048:d7e4.
 *
 * Ported by tools/port.py from matched/game/win_move_to_1048_d7e4.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void win_move_to_1078_1ff8(void *w, col_i16 a, col_i16 b);

void win_move_to_1048_d7e4(GameWinBase *o, col_i16 a, col_i16 b)
{
    win_move_to_1078_1ff8(o->win, a, b);
}
