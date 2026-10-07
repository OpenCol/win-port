/* get_window_rect_1058_bcd5 -- shipped at 1058:bcd5.
 *
 * Ported by tools/port.py from matched/game/get_window_rect_1058_bcd5.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x76a]; RECT rect; } GameWin_local;

void get_window_rect_1058_bcd5(GameWin_local *w, RECT *r)
{
    *r = w->rect;
}
