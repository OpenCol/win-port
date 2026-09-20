/* Sprite_SetHotSpot -- shipped at 1068:7b0f.
 *
 * Ported by tools/port.py from matched/gfx/Sprite_SetHotSpot.c.
 *
 * The file's own Sprite dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void Sprite_SetHotSpot(Sprite *s, col_i16 x, col_i16 y)
{
    s->hx = x;
    s->hy = y;
}
