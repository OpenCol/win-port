/* win_set_icon_1060_61b9 -- shipped at 1060:61b9.
 *
 * Ported by tools/port.py from matched/game/win_set_icon_1060_61b9.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void win_set_icon_1078_2832(void *w, col_i16 a);

void win_set_icon_1060_61b9(GameWinBase *o, col_i16 a)
{
    win_set_icon_1078_2832(o->win, a);
}
