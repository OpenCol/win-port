/* Sprite_InitStruct: candidate for 1068:8f77 (121 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Blanks every field of a Sprite: both rectangles emptied through `SetRect`, the hot spot
 * at +0x10/+0x12 zeroed, the two words at +0x14/+0x16 set to ONE, the colour byte at +0x18
 * and the handle at +0x1a cleared, and the 32-bit source pointer at +0x1c zeroed with a
 * single `MOV DWORD`.
 *
 * `Sprite_ctor` calls it, and so does one other site -- which is why it is a function rather
 * than the constructor's body.
 *
 * The layout is `Sprite_DrawSprite`'s, confirmed field for field: that function reads
 * +0/+2 and +8/+0xa as the two corners, +0x10/+0x12 as the offset, +0x18 as the default
 * colour and +0x1c as the source. The only fields it does not touch are the two set to 1
 * here.
 *
 * Every store reloads the pointer -- eight separate `LES BX,[bp+6]` -- which is what `-Od`
 * gives for eight statements on the same object.
 *
 * RENAMED 2026-09-26 from Sprite_Clear: the Mac build's Sprite::InitStruct(void): this only, blanking every field of the sprite -- the same job GRPort_InitStruct does for a port. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void Sprite_InitStruct(Sprite *s)
{
    SetRect(&s->bounds, 0, 0, 0, 0);
    SetRect(&s->area, 0, 0, 0, 0);
    s->hx = 0;
    s->hy = 0;
    s->scale_x = 1;
    s->scale_y = 1;
    s->key = 0;
    s->handle = 0;
    s->bits = 0L;
}
