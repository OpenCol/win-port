/* Sprite_GetHotSpot: candidate for 1068:7b31 (40 bytes).  Verdict: EXACT.
 *
 * Returns the sprite's hot spot (+0x10, +0x12) through the two pointers.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void Sprite_GetHotSpot(Sprite *s, int *x, int *y)
{
    *x = s->hx;
    *y = s->hy;
}
