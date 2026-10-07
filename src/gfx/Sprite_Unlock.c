/* Sprite_Unlock: candidate for 1068:7ab5 (53 bytes, 10 callers).  Verdict: RELOCATABLE.
 *
 * Unlocks the sprite's pixel handle when it is locked.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void Sprite_Unlock(Sprite *s)
{
    if (s->bits != 0)
        s->bits = (unsigned char *)MSHUnlock(s->handle);
}
