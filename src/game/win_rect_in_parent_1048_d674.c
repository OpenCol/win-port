/* win_rect_in_parent_1048_d674 -- shipped at 1048:d674.
 *
 * Ported by tools/port.py from matched/game/win_rect_in_parent_1048_d674.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void win_rect_in_parent_1078_2644(void *w, col_i16 a, col_i16 b);

void win_rect_in_parent_1048_d674(GameWinBase *o, col_i16 a, col_i16 b)
{
    win_rect_in_parent_1078_2644(o->win, a, b);
}
