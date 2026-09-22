/* Sprite_Unlock -- shipped at 1068:7ab5.
 *
 * Ported by tools/port.py from matched/gfx/Sprite_Unlock.c.
 *
 * The file's own Sprite dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: bits -> src
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSHUnlock(col_i16 handle);

void Sprite_Unlock(Sprite *s)
{
    if (s->src != 0)
        s->src = (unsigned char *)MSHUnlock(s->handle);
}
