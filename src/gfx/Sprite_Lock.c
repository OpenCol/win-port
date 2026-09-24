/* Sprite_Lock -- shipped at 1068:7a80.
 *
 * Ported by tools/port.py from matched/gfx/Sprite_Lock.c.
 *
 * The file's own Sprite dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: bits -> src
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSHLock(col_i16 h);

void Sprite_Lock(Sprite *s)
{
    if (s->src == 0)
        s->src = (unsigned char *)MSHLock(s->handle);
}
