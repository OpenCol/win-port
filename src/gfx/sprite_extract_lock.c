/* sprite_extract_lock -- shipped at 1068:769f.
 *
 * Ported by tools/port.py from matched/gfx/sprite_extract_lock.c.
 *
 * The file's own Sprite dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 ExtractSprite(Sprite *s, void *src, col_i16 transparent, RECT *r);
extern void Sprite_Lock(Sprite *s);
void sprite_extract_lock(Sprite *s, void *src, unsigned char transparent, RECT *r)
{
    s->handle = ExtractSprite(s, src, transparent, r);
    Sprite_Lock(s);
}
