/* win_move_to_rect_1048_d6da -- shipped at 1048:d6da.
 *
 * Ported by tools/port.py from matched/game/win_move_to_rect_1048_d6da.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void win_move_to_rect_1078_221b(void *w, col_i16 a, col_i16 b);

void win_move_to_rect_1048_d6da(GameWinBase *o, col_i16 a, col_i16 b)
{
    win_move_to_rect_1078_221b(o->win, a, b);
}
