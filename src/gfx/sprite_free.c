/* sprite_free -- shipped at 1068:677b.
 *
 * Ported by tools/port.py from matched/gfx/sprite_free.c.
 *
 * The file's own Sprite dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: bits -> src
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i32 MSHUnlock(col_i16 h);
extern col_u16 MSDisposeHandle(col_u16 h);
extern void Sprite_Clear(Sprite *s);

void sprite_free(Sprite *s)
{
    if (s->src != 0)
        s->src = (void *)MSHUnlock(s->handle);
    if (s->handle != 0)
        s->handle = MSDisposeHandle(s->handle);
    Sprite_Clear(s);
}
