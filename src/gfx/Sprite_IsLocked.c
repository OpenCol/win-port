/* Sprite_IsLocked -- shipped at 1068:7aea.
 *
 * Ported by tools/port.py from matched/gfx/Sprite_IsLocked.c.
 *
 * The file's own Sprite dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: data -> src
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

char Sprite_IsLocked(Sprite *s)
{
    return s->src == 0 ? 0 : 1;
}
