/* win_set_word_1e_1058_bb82 -- shipped at 1058:bb82.
 *
 * Ported by tools/port.py from matched/game/win_set_word_1e_1058_bb82.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void win_set_word_1e_1078_2aef(void *w, col_i16 a);

void win_set_word_1e_1058_bb82(GameWinBase *o, col_i16 a)
{
    win_set_word_1e_1078_2aef(o->win, a);
}
