/* Sprite_IsLocked: candidate for 1068:7aea (37 bytes).  Verdict: EXACT.
 *
 * Whether the sprite is locked -- whether its row data (the pointer at +0x1c) is
 * present. Both callers are `Sprite_ChangeColor`, which asks this and, when the answer
 * is no, calls `Sprite_Lock` and warns "Warning: Sprite not locked in ChangeColor".
 *
 * The name is the Macintosh build's: `Sprite::IsLocked(void)` is in its method table
 * alongside `Sprite::Lock(void)` and `Sprite::Unlock(void)`, which this build already
 * spells `Sprite_Lock` and `Sprite_Unlock`. One argument, a bool return, and callers that
 * pair it with `Sprite_Lock` -- the arity and the use agree.
 *
 * Both arms load `AL` before a single epilogue, which by rule 16 is a conditional
 * expression, with the `== 0` test that way round rather than a `!= 0`.
 *
 * THE FIELD IS A HUGE POINTER, and that is a correction. This file used to declare it
 * `long` and carry `flags: -ml -WS -Od -Ot`, concluding that its module was built for the
 * 8086. A pointer is tested in halves under every flag set (rule 38), so declared
 * `` this is EXACT at the default flags and the per-module 8086 claim goes away.
 * `matched/Sprite_ChangeColor.c` had already declared the same field
 * `unsigned char *` -- two files describing one pointer disagreed, and the bytes
 * side with the one that said .
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

char Sprite_IsLocked(Sprite *s)
{
    return s->bits == 0 ? 0 : 1;
}
