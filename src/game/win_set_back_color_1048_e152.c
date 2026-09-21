/* win_set_back_color_1048_e152 -- shipped at 1048:e152.
 *
 * Ported by tools/port.py from matched/game/win_set_back_color_1048_e152.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void win_set_back_color_1078_1ed6(void *w, col_i16 a);

void win_set_back_color_1048_e152(GameWinBase *o, col_i16 a)
{
    win_set_back_color_1078_1ed6(o->win, a);
}
