/* Sprite_CopyOnto: candidate for 1068:7a50 (48 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * One of four forwarders back to back at 1068:79c4, 79f2, 7a22 and 7a50 onto `Sprite_CopySprite`,
 * which merges source sprite `s` into destination `d` at (dx, dy). All four pass the source
 * TWICE -- as Sprite_CopySprite's unused first argument and as `s` -- and they differ only in
 * whether they take an offset and in the fourth argument, 0 or 1.
 *
 * THAT FOURTH ARGUMENT IS DEAD. Sprite_CopySprite names it `unused2` and never reads it, so the
 * 0 and 1 forms do the same thing; the `_1` in a name records the literal, not a behaviour.
 *
 * The offset form with the fourth argument 1 -- behaviourally `sprite_merge_at`.
 *
 * All four calls have a NOP before the `push cs` -- the five-byte form TLINK leaves when it
 * shortens a call to an `extern` -- so Sprite_CopySprite is declared here, not defined
 * (rule 47).
 *
 * RENAMED 2026-09-27 from sprite_merge_at_1: the Mac build's Sprite_CopyOnto, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: sprite_merge(s, s, dest, 1, dx, dy): merges this sprite onto dest at an offset; 4 args (this + Sprite& + 2 shorts); its only outside caller is FortDraw, which on the Mac calls CopyOnto right after CopyTo, and the Windows call order matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void Sprite_CopySprite(void *unused, Sprite *sprite, Sprite *dest, int unused2,
                             int dx, int dy);

void Sprite_CopyOnto(Sprite *s, Sprite *dest, int dx, int dy)
{
    Sprite_CopySprite(s, s, dest, 1, dx, dy);
}
