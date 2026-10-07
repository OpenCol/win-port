/* Sprite_Extract_key_rect: candidate for 1068:769f (67 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: Sprite_Extract_key_rect
 *
 * Extracts a sprite into the record's handle at +0x1a and locks it.
 *
 * Six words of arguments, but not six ints: `(s, src, transparent, r)`, the shape
 * `Sprite_Extract`, `Sprite_Extract_mask` and `Sprite_ExtractRect` already declare for the
 * same callee. A pass-through forwarder cannot show a split (§119), and
 * `sprite_extract_bounds` -- which builds the rectangle itself -- shows it.
 *
 * `transparent` arrives here as an `unsigned char` and is widened on the way out, which is
 * rule 55 from the caller's side: the callee's prototype takes an `int`.
 *
 * RENAMED 2026-09-27 from sprite_extract_lock: 14 bytes = Extract(GRPort&, unsigned char, Rect*); callers AboutWin_ctor, Opening_ctor = Mac overload's callers. Avoid Sprite_Extract_rect: too close to the existing Sprite_ExtractRect (1068:75fa)
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern short Sprite_ExtractSprite(Sprite *s, GRPort *port, int key, RECT *src);
extern void Sprite_Lock(Sprite *s);
void Sprite_Extract_key_rect(Sprite *sprite, void *src, unsigned char transparent, RECT *r)
{
    sprite->handle = Sprite_ExtractSprite(sprite, (GRPort *)src, transparent, r);
    Sprite_Lock(sprite);
}
