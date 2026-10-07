/* Sprite_CopySprite: candidate for 1068:8a75 (1282 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 1282, not the census 1274. The last 8 bytes are the four-entry jump table, and
 * 1068:8f77 starts immediately after it.
 *
 * MERGES ONE SPRITE INTO ANOTHER at an offset, for the four forwarders at 1068:79c4, 79f2, 7a22 and
 * 7a50 (`Sprite_CopyTo` is the one that builds a unit's flag in `PieceDraw_noport`).
 *
 * An empty source does nothing. An empty destination is filled by COPYING the source: its rectangles,
 * origin, colour and handle, with the handle replaced by a copy of its block so the two sprites do not
 * share one.
 *
 * OTHERWISE BOTH ARE RUN-LENGTH ENCODED, one row at a time, into a new block of w * h + h * 4 bytes.
 * Each row is two words -- the first opaque column and the run length -- and then the run's bytes.
 * Per row, two bits say which sprites reach it: neither gives an empty row (0, 0), one gives that
 * sprite's row copied straight through (the source's first column shifted by dx), and both are merged
 * pixel by pixel over the union of the two runs. In the merge the destination's byte is written first,
 * the source's byte wins over it unless it is the source's transparent colour, and where only the
 * source is transparent the destination's transparent colour goes in instead.
 *
 * The result's rectangle is the union of the two, the old block is disposed, and the merged block
 * becomes the destination's.
 *
 * SPELLINGS: the block size's `h * 4` is `h * 2 * 2` (two `ADD DX,DX`, not a shift and not `h + h`,
 * which would reload); the row length is `n = *q = *p`, a chained store through the output first; the
 * row's two run bounds are declared source-first; the row mask and the per-pixel counter are ONE
 * register variable; and the bits field is `char *`, which is what makes its null test an OR of
 * the halves (rule 75, added for this function).
 *
 * RENAMED 2026-09-27 from sprite_merge: Mac Sprite::CopySprite(Sprite&, Sprite&, short, short, short), 988 bytes, against 1282. Callers: Sprite::CopyOnto and CopyTo on both (Windows adds two 0-caller forwarders). Callees: Mac MSCopyHandle, Lock x3, MSAllocateHandle, MSHLock, MSCopyMem x2, Unlock, MSDisposeHandle, MSHUnlock and Lock. Windows has handle_replace_with_copy, Sprite_Lock x4, MSAllocateHandle, MSHLock, MSCopyMem x2, Sprite_Unlock, MSDisposeHandle and MSHUnlock, the same set. Layout: before Sprite::InitStruct on both. Arity: this+5 = 6 = (unused, sprite, dest, unused2, dx, dy).
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"

extern void handle_replace_with_copy(void *p);

/* same object: 4-byte `push cs; call ` */
extern void Sprite_Lock(Sprite *s);
extern void Sprite_Unlock(Sprite *s);

void Sprite_CopySprite(Sprite *unused, Sprite *sprite, Sprite *dest, int unused2,
                                int dx, int dy)
{
    int y;
    int xoff;
    int yoff;
    int aw;
    int ah;
    int bw;
    int bh;
    int w;
    int h;
    int s0, s1;
    int d0, d1;
    int n;
    RECT r;
    short hnew;
    char *pa;
    char *pb;
    char *q;
    register int k;
    register int j;

    if (sprite->handle == 0)
        return;
    if (dest->handle == 0) {
        SetRect((RECT *)dest, sprite->bounds.left, sprite->bounds.top, sprite->bounds.right + dx,
                sprite->bounds.bottom + dy);
        dest->area = sprite->area;
        OffsetRect(&dest->area, dx, dy);
        dest->hx = sprite->hx + dx;
        dest->hy = sprite->hy + dy;
        dest->scale_x = sprite->scale_x;
        dest->scale_y = sprite->scale_y;
        dest->key = sprite->key;
        dest->handle = sprite->handle;
        handle_replace_with_copy((Handle *)&dest->handle);
        Sprite_Lock(dest);
        return;
    }
    xoff = dx;
    yoff = sprite->area.top + dy - dest->area.top;
    aw = rect_width_1068_96a9(&sprite->area) + sprite->area.left;
    ah = rect_height_1068_96c8(&sprite->area);
    bw = rect_width_1068_96a9(&dest->area) + dest->area.left;
    bh = rect_height_1068_96c8(&dest->area);
    SetRect(&r, 0, 0, max_int_1068_98e4(bw, aw + xoff), max_int_1068_98e4(bh, ah + yoff));
    w = r.right;
    h = r.bottom;
    Sprite_Lock(sprite);
    Sprite_Lock(dest);
    pa = (char *)sprite->bits;
    pb = (char *)dest->bits;
    hnew = MSAllocateHandle((long)w * h + (unsigned)(h * 2 * 2));
    q = (char *)MSHLock(hnew);
    for (y = 0; y < h; y++) {
        k = 0;
        if (y < bh)
            k |= 1;
        if (y >= yoff && yoff + ah > y)
            k |= 2;
        switch (k) {
        case 0:
            *(int *)q = 0;
            q += 2;
            *(int *)q = 0;
            q += 2;
            break;
        case 1:
            *(int *)q = *(int *)pb;
            q += 2;
            pb += 2;
            n = *(int *)q = *(int *)pb;
            q += 2;
            pb += 2;
            MSCopyMem(pb, q, (long)n);
            pb += n;
            q += n;
            break;
        case 2:
            *(int *)q = *(int *)pa + xoff;
            q += 2;
            pa += 2;
            n = *(int *)q = *(int *)pa;
            q += 2;
            pa += 2;
            MSCopyMem(pa, q, (long)n);
            pa += n;
            q += n;
            break;
        case 3:
            d0 = *(int *)pb;
            pb += 2;
            d1 = *(int *)pb + d0;
            pb += 2;
            s0 = *(int *)pa + xoff;
            pa += 2;
            s1 = *(int *)pa + s0;
            pa += 2;
            *(int *)q = min_int_1068_9847(d0, s0);
            q += 2;
            *(int *)q = max_int_1068_98e4(d1, s1) - *(int *)(q - 2);
            q += 2;
            n = w;
            for (j = 0; n-- != 0; j++) {
                k = 0;
                if (j >= d0 && j < d1) {
                    *q = *pb;
                    pb++;
                    k++;
                }
                if (j >= s0 && j < s1) {
                    if ((int)*pa != sprite->key)
                        *q = *pa;
                    else if (k == 0)
                        *q = dest->key;
                    pa++;
                    k++;
                }
                if (k != 0)
                    q++;
            }
            break;
        }
    }
    OffsetRect(&r, dest->area.left, dest->area.top);
    dest->area = r;
    dest->bounds = r;
    if (dest->bits != 0)
        Sprite_Unlock(dest);
    if (dest->handle != 0)
        dest->handle = MSDisposeHandle(dest->handle);
    MSHUnlock(hnew);
    dest->handle = hnew;
    Sprite_Lock(dest);
}
