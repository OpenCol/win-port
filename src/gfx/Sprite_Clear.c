/* Sprite_Clear -- shipped at 1068:8f77.
 *
 * Ported by tools/port.py from matched/gfx/Sprite_Clear.c.
 *
 * The file's own Sprite dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: ox -> hx, oy -> hy
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

void Sprite_Clear(Sprite *s)
{
    SetRect(&(*((RECT *)((unsigned char *)(s) + 0x0))), 0, 0, 0, 0);
    SetRect(&(*((RECT *)((unsigned char *)(s) + 0x8))), 0, 0, 0, 0);
    s->hx = 0;
    s->hy = 0;
    (*((col_i16 *)((unsigned char *)(s) + 0x14))) = 1;
    (*((col_i16 *)((unsigned char *)(s) + 0x16))) = 1;
    s->colour = 0;
    s->handle = 0;
    s->src = 0L;
}
