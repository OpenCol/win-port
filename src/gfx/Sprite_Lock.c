/* Sprite_Lock: candidate for 1068:7a80 (53 bytes, 17 callers).  Verdict: RELOCATABLE.
 *
 * Locks the sprite's pixel handle into its pointer when not already locked.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void Sprite_Lock(Sprite *s)
{
    if (s->bits == 0)
        s->bits = (unsigned char *)MSHLock(s->handle);
}
