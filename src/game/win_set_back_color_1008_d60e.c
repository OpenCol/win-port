/* win_set_back_color_1008_d60e -- shipped at 1008:d60e.
 *
 * Ported by tools/port.py from matched/game/win_set_back_color_1008_d60e.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void win_set_back_color_1078_1ed6(void *w, col_i16 a);

void win_set_back_color_1008_d60e(GameWinBase *o, col_i16 a)
{
    win_set_back_color_1078_1ed6(o->win, a);
}
