/* Sprite_SetHotSpot: candidate for 1068:7b0f (34 bytes).  Verdict: EXACT.
 *
 * The sprite's hot spot: x at +0x10, y at +0x12. Sprite_GetHotSpot right after it reads the
 * same two words.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void Sprite_SetHotSpot(Sprite *s, int x, int y)
{
    s->hx = x;
    s->hy = y;
}
