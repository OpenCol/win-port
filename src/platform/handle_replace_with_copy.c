/* handle_replace_with_copy: candidate for 1070:cfea (137 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Replaces the object's global handle with a fresh copy of the same size: ask `GlobalSize`,
 * get a new handle from `MSAllocateHandle`, lock both, `hmemcpy` across, unlock both, store
 * the new handle. A failed allocation leaves the old handle in place.
 *
 * IT DOES NOT FREE THE OLD HANDLE. The original is unlocked and then simply dropped, so
 * every call leaks a block the size of the one it copied. That is what the bytes do and it
 * is left alone.
 *
 * `GlobalSize` returns a long in DX:AX and the shipped rebuilds it with `SHL EAX,16 /
 * SHRD EAX,EDX,16 / MOV [bp-6],EAX` -- Borland's 386 idiom for combining a 32-bit API return
 * into one register.
 *
 * THE SIGNEDNESS MISMATCH IS WHAT PRODUCES IT, and it is the whole reason this file has a
 * `long n` under an `unsigned long` return. Declare both the same way, either way round, and
 * Borland stores the halves instead -- `MOV [bp-4],DX / MOV [bp-6],AX` -- six bytes of
 * difference. Widening `hmemcpy`'s third parameter to `unsigned` instead of `unsigned long`
 * does the same. So the shipped source assigned a `DWORD` API result to a signed `long`, and
 * the conversion is what routes the value through EAX.
 */

#include "colonization.h"

#include "records.h"

void handle_replace_with_copy(Handle *h)
{
    long size;
    void *src;
    void *dst;
    register unsigned nh;

    size = GlobalSize(h->handle);
    nh = MSAllocateHandle(size);
    if (nh != 0) {
        dst = GlobalLock(nh);
        src = GlobalLock(h->handle);
        hmemcpy(dst, src, size);
        GlobalUnlock(nh);
        GlobalUnlock(h->handle);
        h->handle = nh;
    }
}
