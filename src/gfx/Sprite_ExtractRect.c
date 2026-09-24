/* Sprite_ExtractRect -- shipped at 1068:75fa.
 *
 * Ported by tools/port.py from matched/gfx/Sprite_ExtractRect.c.
 *
 * The file's own Sprite dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 ExtractSprite(Sprite *s, void *src, col_i16 transparent, RECT *r);
extern void Sprite_Lock(Sprite *s);

void Sprite_ExtractRect(Sprite *s, void *src, RECT *r)
{
    s->handle = ExtractSprite(s, src, -1, r);
    Sprite_Lock(s);
}
