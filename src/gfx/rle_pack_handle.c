/* rle_pack_handle -- shipped at 1068:c831.
 *
 * Ported by tools/port.py from matched/gfx/rle_pack_handle.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef unsigned char *hp;
extern col_u16 MSAllocateHandle(col_u32 size);
extern void * MSHLock(col_i16 h);
extern void * MSHUnlock(col_i16 h);
extern col_i16 MSDisposeHandle(col_u16 h);
extern void MSCopyMem(void *src, void *dst, col_i32 n);

col_i32 rle_pack_handle(col_i16 *h, col_i32 n)
{
    hp start;
    hp out;
    hp src;
    hp lit;
    hp p;
    hp end;
    col_i32 total;
    col_i32 cnt;
    col_i32 run;
    register col_i16 scratch;

    scratch = MSAllocateHandle(n / 0x80 + n + 5);
    start = out = (hp)MSHLock(scratch);
    lit = p = src = (hp)MSHLock(*h);
    p++;
    end = src + n;
    while (lit < end) {
        while (p < end && *p == *lit)
            p++;
        run = p - lit;
        if (run > 2) {
            while (run != 0) {
                if (run > 0x7f) {
                    *out = 0x7f;
                    out++;
                    run -= 0x7f;
                } else {
                    *out = (unsigned char)run;
                    out++;
                    run = 0;
                }
                *out = *lit;
                out++;
            }
            lit = p;
        }
        while (p < end && p[1] != *p)
            p++;
        run = p - lit;
        while (run != 0) {
            if (run > 0x7f) {
                *out = 0xff;
                out++;
                run -= 0x7f;
                cnt = 0x7f;
            } else {
                *out = (unsigned char)run + 0x80;
                out++;
                cnt = run;
                run = 0;
            }
            while (cnt-- != 0) {
                *out = *lit;
                lit++;
                out++;
            }
        }
    }
    total = out - start;
    MSHUnlock(*h);
    MSDisposeHandle(*h);
    *h = MSAllocateHandle(total);
    src = start;
    start = (hp)MSHLock(*h);
    MSCopyMem(src, start, total);
    MSHUnlock(scratch);
    MSDisposeHandle(scratch);
    return total;
}
