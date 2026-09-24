/* win_set_cursor_1038_e30a -- shipped at 1038:e30a.
 *
 * Ported by tools/port.py from matched/game/win_set_cursor_1038_e30a.c.
 *
 * The file's own CursorClass, Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void win_set_cursor_1078_2986(void *win, col_i16 cursor);

void win_set_cursor_1038_e30a(Win *w, CursorClass *c)
{
    win_set_cursor_1078_2986((*((void * *)((unsigned char *)(w) + 0x6))), c->hcursor);
}
