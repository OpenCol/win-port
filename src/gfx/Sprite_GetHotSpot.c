/* Sprite_GetHotSpot -- shipped at 1068:7b31.
 *
 * Ported by tools/port.py from matched/gfx/Sprite_GetHotSpot.c.
 *
 * The file's own Sprite dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void Sprite_GetHotSpot(Sprite *s, col_i16 *x, col_i16 *y)
{
    *x = s->hx;
    *y = s->hy;
}
