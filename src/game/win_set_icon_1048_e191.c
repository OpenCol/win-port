/* win_set_icon_1048_e191 -- shipped at 1048:e191.
 *
 * Ported by tools/port.py from matched/game/win_set_icon_1048_e191.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void win_set_icon_1078_2832(void *w, col_i16 a);

void win_set_icon_1048_e191(GameWinBase *o, col_i16 a)
{
    win_set_icon_1078_2832(o->win, a);
}
