/* slot_rect -- shipped at 1048:4dbc.
 *
 * Ported by tools/port.py from matched/game/slot_rect.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void slot_rect(col_i16 i, col_i16 *x, col_i16 *y, col_i16 *w, col_i16 *h)
{
    *x = i * 0x18 + 0x122;
    *y = 0x171;
    *w = 10;
    *h = 12;
}
