/* win_load_cursor_1038_e331 -- shipped at 1038:e331.
 *
 * Ported by tools/port.py from matched/game/win_load_cursor_1038_e331.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void win_load_cursor_1078_29b6(void *w, col_i16 a);

void win_load_cursor_1038_e331(GameWinBase *o, col_i16 a)
{
    win_load_cursor_1078_29b6(o->win, a);
}
