/* sprite_merge -- shipped at 1068:8a75.
 *
 * Ported by tools/port.py from matched/gfx/sprite_merge.c.
 *
 * The file's own Sprite dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: bits -> src, ox -> hx, oy -> hy
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1068_98e4(col_i16 a, col_i16 b);
col_i16 min_int_1068_9847(col_i16 a, col_i16 b);
col_i16 rect_height_1068_96c8(RECT *r);
col_i16 rect_width_1068_96a9(RECT *r);
void Sprite_Lock(Sprite *s);
void Sprite_Unlock(Sprite *s);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal OffsetRect(RECT *r, col_i16 dx, col_i16 dy);
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void MSCopyMem(void *src, void *dst, col_i32 n);
extern col_u16 MSAllocateHandle(col_u32 size);
extern void * MSHLock(col_i16 h);
extern void * MSHUnlock(col_i16 h);
extern col_i16 MSDisposeHandle(col_u16 h);
extern void handle_replace_with_copy(void *p);

/* same object: 4-byte `push cs; call ` */







void sprite_merge(void *unused, Sprite *s, Sprite *d, col_i16 unused2,
                                col_i16 dx, col_i16 dy)
{
    col_i16 y;
    col_i16 xoff;
    col_i16 yoff;
    col_i16 aw;
    col_i16 ah;
    col_i16 bw;
    col_i16 bh;
    col_i16 w;
    col_i16 h;
    col_i16 s0, s1;
    col_i16 d0, d1;
    col_i16 n;
    RECT r;
    col_i16 hnew;
    char *pa;
    char *pb;
    char *q;
    register col_i16 k;
    register col_i16 j;

    if (s->handle == 0)
        return;
    if (d->handle == 0) {
        SetRect((RECT *)d, (*((RECT *)((unsigned char *)(s) + 0x0))).left, (*((RECT *)((unsigned char *)(s) + 0x0))).top, (*((RECT *)((unsigned char *)(s) + 0x0))).right + dx,
                (*((RECT *)((unsigned char *)(s) + 0x0))).bottom + dy);
        (*((RECT *)((unsigned char *)(d) + 0x8))) = (*((RECT *)((unsigned char *)(s) + 0x8)));
        OffsetRect(&(*((RECT *)((unsigned char *)(d) + 0x8))), dx, dy);
        d->hx = s->hx + dx;
        d->hy = s->hy + dy;
        (*((col_i16 *)((unsigned char *)(d) + 0x14))) = (*((col_i16 *)((unsigned char *)(s) + 0x14)));
        (*((col_i16 *)((unsigned char *)(d) + 0x16))) = (*((col_i16 *)((unsigned char *)(s) + 0x16)));
        d->colour = s->colour;
        d->handle = s->handle;
        handle_replace_with_copy(&d->handle);
        Sprite_Lock(d);
        return;
    }
    xoff = dx;
    yoff = (*((RECT *)((unsigned char *)(s) + 0x8))).top + dy - (*((RECT *)((unsigned char *)(d) + 0x8))).top;
    aw = rect_width_1068_96a9(&(*((RECT *)((unsigned char *)(s) + 0x8)))) + (*((RECT *)((unsigned char *)(s) + 0x8))).left;
    ah = rect_height_1068_96c8(&(*((RECT *)((unsigned char *)(s) + 0x8))));
    bw = rect_width_1068_96a9(&(*((RECT *)((unsigned char *)(d) + 0x8)))) + (*((RECT *)((unsigned char *)(d) + 0x8))).left;
    bh = rect_height_1068_96c8(&(*((RECT *)((unsigned char *)(d) + 0x8))));
    SetRect(&r, 0, 0, max_int_1068_98e4(bw, aw + xoff), max_int_1068_98e4(bh, ah + yoff));
    w = r.right;
    h = r.bottom;
    Sprite_Lock(s);
    Sprite_Lock(d);
    pa = (char *)s->src;
    pb = (char *)d->src;
    hnew = MSAllocateHandle((col_i32)w * h + (col_u16)(h * 2 * 2));
    q = (char *)MSHLock(hnew);
    for (y = 0; y < h; y++) {
        k = 0;
        if (y < bh)
            k |= 1;
        if (y >= yoff && yoff + ah > y)
            k |= 2;
        switch (k) {
        case 0:
            *(col_i16 *)q = 0;
            q += 2;
            *(col_i16 *)q = 0;
            q += 2;
            break;
        case 1:
            *(col_i16 *)q = *(col_i16 *)pb;
            q += 2;
            pb += 2;
            n = *(col_i16 *)q = *(col_i16 *)pb;
            q += 2;
            pb += 2;
            MSCopyMem(pb, q, (col_i32)n);
            pb += n;
            q += n;
            break;
        case 2:
            *(col_i16 *)q = *(col_i16 *)pa + xoff;
            q += 2;
            pa += 2;
            n = *(col_i16 *)q = *(col_i16 *)pa;
            q += 2;
            pa += 2;
            MSCopyMem(pa, q, (col_i32)n);
            pa += n;
            q += n;
            break;
        case 3:
            d0 = *(col_i16 *)pb;
            pb += 2;
            d1 = *(col_i16 *)pb + d0;
            pb += 2;
            s0 = *(col_i16 *)pa + xoff;
            pa += 2;
            s1 = *(col_i16 *)pa + s0;
            pa += 2;
            *(col_i16 *)q = min_int_1068_9847(d0, s0);
            q += 2;
            *(col_i16 *)q = max_int_1068_98e4(d1, s1) - *(col_i16 *)(q - 2);
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
                    if ((col_i16)*pa != s->colour)
                        *q = *pa;
                    else if (k == 0)
                        *q = d->colour;
                    pa++;
                    k++;
                }
                if (k != 0)
                    q++;
            }
            break;
        }
    }
    OffsetRect(&r, (*((RECT *)((unsigned char *)(d) + 0x8))).left, (*((RECT *)((unsigned char *)(d) + 0x8))).top);
    (*((RECT *)((unsigned char *)(d) + 0x8))) = r;
    (*((RECT *)((unsigned char *)(d) + 0x0))) = r;
    if (d->src != 0)
        Sprite_Unlock(d);
    if (d->handle != 0)
        d->handle = MSDisposeHandle(d->handle);
    MSHUnlock(hnew);
    d->handle = hnew;
    Sprite_Lock(d);
}
