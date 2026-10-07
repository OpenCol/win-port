/* handle_resize: candidate for 1070:d171 (163 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Grows or shrinks the handle a record holds at +0, by allocating a new one and copying:
 * both blocks are locked, `min_long_1070_de2b` picks the smaller of the old block's size and
 * the new one, `MSCopyMem` moves that many bytes, both are unlocked, the old handle is freed
 * and the new one written back over it.
 *
 * The clamp is what makes it safe in both directions -- a shrink copies only the new size, a
 * grow only the old -- and it is a `long` comparison, so blocks over 64K are handled.
 *
 * `MSBlockSize` is asked about the OLD handle after the new one is already locked, which is
 * the shipped order and costs nothing: neither lock moves anything.
 *
 * Everything here is -called and in this translation unit, so all seven helpers are stubs
 * (rule 47); the record's handle is a word and the sizes are `long`s.
 *
 * The three locals are declared size-then-old-then-new even though the size is fetched LAST:
 * Borland gives the first-declared local the highest frame slot, and the shipped keeps the
 * size at `[bp-6]`, the old pointer at `[bp-0xa]` and the new at `[bp-0xe]`. Declared in the
 * order they are assigned, every frame reference moves by four.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` */

void handle_resize(short *hp, long size)
{
    long n;
    void *old;
    void *nw;
    register short h;

    h = MSAllocateHandle(size);
    old = MSHLock(*hp);
    nw = MSHLock(h);
    n = MSBlockSize(*hp);
    MSCopyMem(old, nw, min_long_1070_de2b(n, size));
    MSHUnlock(h);
    MSHUnlock(*hp);
    MSDisposeHandle(*hp);
    *hp = h;
}
