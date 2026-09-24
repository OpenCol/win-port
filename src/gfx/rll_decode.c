/* rll_decode -- shipped at 1068:c53b.
 *
 * Ported by tools/port.py from matched/gfx/rll_decode.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void empty_1068_dc3e(void *ctx, char *msg);

typedef unsigned char *hp;

extern col_u32 MSBlockSize(col_i16 h);
extern col_u16 MSAllocateHandle(col_u32 size);
extern void * MSHLock(col_i16 h);
extern void * MSHUnlock(col_i16 h);
extern col_i16 MSDisposeHandle(col_u16 h);
extern void MSCopyMem(void *src, void *dst, col_i32 n);
extern void handle_resize(col_i16 *h, col_i32 size);

/* same object: 4-byte `push cs; call ` (rule 47) */


col_i32 rll_decode(col_i16 *h)
{
    hp dend;
    hp send;
    hp src_base;
    hp dst_base;
    hp sp;
    hp dp;
    col_i16 val;
    col_i32 cap;
    col_u32 size;
    col_i32 out;
    register col_i16 n;
    register col_i16 scratch;

    size = MSBlockSize(*h);
    scratch = MSAllocateHandle(size);
    if (scratch == 0) {
        empty_1068_dc3e(&(*g_error_ctx), "RLLDecode could not allocate decompression buffer");
        return 0L;
    }
    src_base = (hp)MSHLock(scratch);
    dst_base = (hp)MSHLock(*h);
    MSCopyMem(dst_base, src_base, size);
    cap = size << 1;
    MSHUnlock(*h);
    handle_resize(h, cap);
    dst_base = (hp)MSHLock(*h);
    send = src_base + size;
    dend = dst_base + cap;
    dp = dst_base;
    sp = src_base;
    out = 0L;
    while (send - 1 > sp) {
        n = *sp;
        sp++;
        if (dp + (n & 0x7f) + 1 > dend) {
            MSHUnlock(*h);
            cap += 1000;
            handle_resize(h, cap);
            dst_base = (hp)MSHLock(*h);
            dp = dst_base + out;
            dend = dst_base + cap;
        }
        if (n < 0x80) {
            val = *sp;
            sp++;
            while (n-- != 0) {
                *dp = val;
                dp++;
                out++;
            }
        } else {
            n -= 0x80;
            while (n-- != 0) {
                *dp = *sp;
                sp++;
                dp++;
                out++;
            }
        }
    }
    dst_base = dp = (hp)MSHUnlock(*h);
    handle_resize(h, out);
    src_base = sp = (hp)MSHUnlock(scratch);
    scratch = MSDisposeHandle(scratch);
    return out;
}
