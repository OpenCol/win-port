/* Sprite_Extract_mask -- shipped at 1068:782a.
 *
 * Ported by tools/port.py from matched/gfx/Sprite_Extract_mask.c.
 *
 * The file's own Sprite dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern col_i16 ExtractSprite(Sprite *s, void *src, col_i16 transparent, RECT *r);
extern void Sprite_Lock(Sprite *s);

void Sprite_Extract_mask(Sprite *s, void *src, unsigned char transparent, col_i16 x, col_i16 y, col_i16 w, col_i16 h)
{
    RECT rc;

    SetRect(&rc, x, y, x + w, y + h);
    s->handle = ExtractSprite(s, src, transparent, &rc);
    Sprite_Lock(s);
}
