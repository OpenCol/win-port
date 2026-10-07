/* Sprite_Init: candidate for 1068:6859 (554 bytes).  Verdict: RELOCATABLE.
 * name-source: 37 callers over 63 sites, all passing one word; the error message names the resource kind
 *
 * LOADS A SPRITE FROM A SPRT RESOURCE. A missing resource only prints
 * "Error: SPRT resource not found - " with the id and leaves the sprite alone.
 *
 * The resource is read field by field through a `` pointer that is stepped past each one: two
 * byte-swapped words into the sprite's +0x10 and +0x12, two 8-byte rectangles into +0 and +8, the
 * transparent colour at +0x18, and a long that is the size of the pixel block. Whatever the sprite held
 * before is unlocked and disposed, a handle of that size is allocated and locked, the rest of the
 * resource is copied into it, and `swap_sprite_row_words` walks it once for as many rows as the second
 * rectangle's height (bottom - top).
 *
 * The resource is then unlocked and freed, the walking pointer is cleared, and the sprite is locked.
 *
 * `bits` at +0x1c is a `` pointer, so its null test is the OR of the two halves (rule 75), and
 * `Sprite_Lock` and `Sprite_Unlock` are reached by the NOP form and so are extern (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void Sprite_Lock(Sprite *s);
extern void Sprite_Unlock(Sprite *s);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void *MSGetResource_1068_94c7(void *type, int id);
extern void report_error_1068_94ea(void *ctx, char *msg, long v);
extern void *MSLockResource_1068_94f8(void *h);

extern void MSUnlockResource_1068_952d(void *res);
extern void MSReleaseResource_1068_9549(void *res);

extern void swap_sprite_row_words(void *p, int n);

void Sprite_Init(Sprite *s, register int res_id)
{
    void *h;
    long type;
    char *p;
    void *bits;
    long size;

    type = g_sprt_type;
    h = MSGetResource_1068_94c7(&type, res_id);
    if (h == 0) {
        report_error_1068_94ea(&g_error_log, "Error: SPRT resource not found - ", (long)(unsigned)res_id);
        return;
    }
    p = (char *)MSLockResource_1068_94f8(h);
    s->hx = swap_word_identity_1068_9518(*(int *)p);
    p += 2;
    s->hy = swap_word_identity_1068_9518(*(int *)p);
    p += 2;
    copy_rect((RECT *)p, &s->bounds);
    p += 8;
    copy_rect((RECT *)p, &s->area);
    p += 8;
    s->key = *p;
    p += 1;
    size = identity_ptr_1068_9991(*(long *)p);
    p += 4;
    if (s->bits != 0)
        Sprite_Unlock(s);
    if (s->handle != 0)
        s->handle = MSDisposeHandle(s->handle);
    s->handle = MSAllocateHandle(size);
    bits = MSHLock(s->handle);
    MSCopyMem(p, bits, size);
    swap_sprite_row_words((short *)bits, rect_height_1068_96c8(&s->area));
    bits = MSHUnlock(s->handle);
    MSUnlockResource_1068_952d(h);
    p = 0;
    MSReleaseResource_1068_9549(h);
    Sprite_Lock(s);
}
