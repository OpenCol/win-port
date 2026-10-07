/* Sprite_Draw: candidate for 1068:7894 (58 bytes, 187 call sites in 71 functions).
 * Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * `Sprite_DrawSprite` with -1 forced into the colour slot: draw the sprite unrecoloured,
 * into `out`, at (px, py) of the port, and hand back the rectangle pointer it was given
 * (DX:AX is reloaded from [BP+8]:[BP+6]).
 *
 * ARGUMENT SPLIT CORRECTED. This file used to declare `(void *, int, int, int, int,
 * void *)` -- the right sixteen bytes divided in the wrong places, which is exactly
 * §119's blind spot: two adjacent word pushes and a pointer are the same instructions,
 * so a green verdict cannot tell them apart. The callers can. Ten reconstructions call this
 * as `Sprite_Draw(&rect, sprite, window, x, y)` and `Sprite_DrawSprite`, which was read
 * instruction by instruction, takes `(RECT *out, Sprite *s, GRPort *port, int
 * colour, int px, int py)`. The four sibling wrappers at 1068:78ce, 790c, 794d and 798a
 * carried the same wrong split and are corrected with it.
 *
 * Note the NOP before PUSH CS at 1068:78b8 -- that is TLINK's -call optimisation,
 * `9A off seg` rewritten as `90 0E E8 rel`, so the call was a plain call in the object
 * and the callee happened to land in the same physical segment.
 *
 * RENAMED 2026-09-26 from sprite_draw: the Mac build's Sprite::Draw(GRPort&, short, short): the sprite is `this` (the RECT pointer before it is the hidden return slot), then the port, x and y, in that order.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
RECT *Sprite_DrawSprite(RECT *out, Sprite *sprite, GRPort *port,
                                 int colour, int px, int py);

RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py)
{
    Sprite_DrawSprite(out, s, port, -1, px, py);
    return out;
}
