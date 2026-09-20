/* in_range_mode -- shipped at 1038:b52a.
 *
 * Ported by tools/port.py from matched/game/in_range_mode.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 in_range_mode(col_i16 dx, col_i16 dy, col_i16 mode)
{
    col_i16 r;

    dx = dx > 0 ? dx : ~dx + 1;
    dy = dy > 0 ? dy : ~dy + 1;
    r = (dx + dy <= 1);
    if (mode == 1) goto done;
    r |= (dx < 2 && dy < 2);
    if (mode == 2) goto done;
    r |= (dx + dy <= 2);
    if (mode == 3) goto done;
    r |= (dx < 2 || dy < 2);
done:
    return r;
}
