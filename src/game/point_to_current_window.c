/* point_to_current_window -- shipped at 1060:0839.
 *
 * Ported by tools/port.py from matched/game/point_to_current_window.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern char * current_window(void);

void point_to_current_window(col_i16 x, col_i16 y)
{
    Win *c;
    char *w = current_window();

    c = (Win *)(w != 0 ? w - 0x26 : w);
    if (c->left > x || c->right < x || c->top > y || c->bottom < y)
        return;
    x -= c->left;
    y -= c->top;
}
