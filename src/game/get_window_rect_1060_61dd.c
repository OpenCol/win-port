/* get_window_rect_1060_61dd -- shipped at 1060:61dd.
 *
 * Ported by tools/port.py from matched/game/get_window_rect_1060_61dd.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x76a]; RECT rect; } GameWin_local;

void get_window_rect_1060_61dd(GameWin_local *w, RECT *r)
{
    *r = w->rect;
}
