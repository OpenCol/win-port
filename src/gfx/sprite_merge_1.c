/* PORT: this file's own declarations of Sprite_CopySprite removed: the header declares it from the definition (W1 step 3). */
/* sprite_merge_1: candidate for 1068:7a22 (46 bytes).  Verdict: RELOCATABLE.
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
 * The no-offset form with the fourth argument 1 -- behaviourally `Sprite_CopyTo`.
 *
 * All four calls have a NOP before the `push cs` -- the five-byte form TLINK leaves when it
 * shortens a call to an `extern` -- so Sprite_CopySprite is declared here, not defined
 * (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void sprite_merge_1(Sprite *sprite, Sprite *dest)
{
    Sprite_CopySprite(sprite, sprite, dest, 1, 0, 0);
}
