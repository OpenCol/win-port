/* Sprite_CopyTo: candidate for 1068:79c4 (46 bytes).  Verdict: RELOCATABLE.
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
 * The no-offset, fourth-argument-0 form -- the one `PieceDraw_noport` uses to build a unit's
 * flag, which is where its name comes from. It was `sprite_set_art_1068_79c4`; the stem is
 * unique, so the suffix said nothing.
 *
 * All four calls have a NOP before the `push cs` -- the five-byte form TLINK leaves when it
 * shortens a call to an `extern` -- so Sprite_CopySprite is declared here, not defined
 * (rule 47).
 *
 * RENAMED 2026-09-26 from sprite_set_art: Sprite_CopySprite(s, s, dest, 0, 0, 0): merges source sprite s into dest at offset (0,0). dest's pixels stay wherever s is transparent, and the result's rect is the union of the two. dest's art is replaced only when dest was empty. It is the no-offset, 0-form member of the sprite_merge_at / sprite_merge_1 / Sprite_CopyOnto family. 'set_art' says it replaces the art, which it does not.
 *
 * RENAMED 2026-09-27 from sprite_merge_onto: the Mac build's Sprite_CopyTo, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: sprite_merge(s, s, dest, 0, 0, 0): merges this sprite onto dest at (0,0); 2 args (this + Sprite&); called by FortDraw, PieceDraw, TeepeeDraw and compose_unit_icon, the Mac CopyTo callers; its siblings sprite_merge_1 and sprite_merge_at have no outside callers.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void Sprite_CopySprite(void *unused, Sprite *sprite, Sprite *dest, int unused2,
                             int dx, int dy);

void Sprite_CopyTo(Sprite *s, Sprite *dest)
{
    Sprite_CopySprite(s, s, dest, 0, 0, 0);
}
