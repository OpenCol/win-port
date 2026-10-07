/* sprite_free: candidate for 1068:677b (99 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Releases a Sprite's pixels: unlock the handle at +0x1a if the sprite is locked, free the
 * handle, then `Sprite_InitStruct` to zero the rest of the record. Both stores write the return
 * value back -- `MSHUnlock` returns 0L into the pointer at +0x1c and `MSDisposeHandle`
 * returns 0 into the handle -- which is how a freed sprite ends up describing itself
 * correctly rather than holding a dangling handle.
 *
 * The locked test reads the pointer as TWO HALVES (`MOV AX,[+0x1c] / OR AX,[+0x1e]`), not as
 * one 32-bit compare, which is what says +0x1c is a `` pointer: a pointer under `-3`
 * tests as one `CMP DWORD`, and a one cannot, because it may be unnormalised. The same
 * discrimination settled `MSLockGraf`'s surface.
 *
 * The handle test is a plain 16-bit compare, so the handle is a word.
 *
 * `Sprite_InitStruct` is reached by the five-byte NOP form and is `extern` (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void Sprite_InitStruct(Sprite *s);

void sprite_free(Sprite *sprite)
{
    if (sprite->bits != 0)
        sprite->bits = (unsigned char *)MSHUnlock(sprite->handle);
    if (sprite->handle != 0)
        sprite->handle = MSDisposeHandle(sprite->handle);
    Sprite_InitStruct(sprite);
}
