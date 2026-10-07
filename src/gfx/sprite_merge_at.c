/* sprite_merge_at: candidate for 1068:79f2 (48 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * One of four forwarders back to back at 1068:79c4, 79f2, 7a22 and 7a50 onto `Sprite_CopySprite`,
 * which merges source sprite `s` into destination `d` at (dx, dy). All four pass the source
 * TWICE -- as Sprite_CopySprite's unused first argument and as `s` -- and they differ only in
 * whether they take an offset and in the fourth argument, 0 or 1.
 *
 * THAT FOURTH ARGUMENT IS DEAD. Sprite_CopySprite names it `unused2` and never reads it, so the
 * 0 and 1 forms do the same thing; the `_1` in a name records the literal, not a behaviour.
 *
 * The form that takes an offset, with the fourth argument 0.
 *
 * All four calls have a NOP before the `push cs` -- the five-byte form TLINK leaves when it
 * shortens a call to an `extern` -- so Sprite_CopySprite is declared here, not defined
 * (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void Sprite_CopySprite(void *unused, Sprite *sprite, Sprite *dest, int unused2,
                             int dx, int dy);

void sprite_merge_at(Sprite *sprite, Sprite *dst, int dx, int dy)
{
    Sprite_CopySprite(sprite, sprite, dst, 0, dx, dy);
}
