/* win_set_cursor_1038_e30a -- shipped at 1038:e30a.
 *
 * Ported by tools/port.py from matched/game/win_set_cursor_1038_e30a.c.
 *
 * The file's own CursorClass dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void win_set_cursor_1078_2986(void *win, col_i16 cursor);

void win_set_cursor_1038_e30a(GameWinBase *w, CursorClass *c)
{
    win_set_cursor_1078_2986(w->win, c->hcursor);
}
