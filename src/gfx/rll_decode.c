/* rll_decode: candidate for 1068:c53b (758 bytes).  Verdict: RELOCATABLE.
 * size-source: 758, not the census 749. The trailing 9 bytes are the dead second epilogue.
 *
 * "RLLDecode": unpacks a memory handle in place, the inverse of `rle_pack_handle`, and
 * returns the unpacked length. The packed bytes are copied to a scratch handle first, and the
 * caller's handle is resized to twice the packed size to receive the output. Each run header below
 * 0x80 is a repeat count followed by the byte to repeat; 0x80 + n is followed by n literal bytes.
 * Whenever a run would pass the end of the output, the handle grows by 1000 bytes and the output
 * pointers are rebuilt from the new lock. At the end the handle is cut to the unpacked size and the
 * scratch handle freed. With no memory for the scratch copy it reports "RLLDecode could not
 * allocate decompression buffer" to the warning context at DGROUP:0x2ac0 and answers 0.
 *
 * The loop stops one byte before the end of the input, so a packed stream's final byte is never
 * read as a header.
 *
 * `size` is a `long`, as MSBlockSize returns (2026-10-02): an `unsigned long` holding it is
 * the shl/shrd conversion once the module declares MSBlockSize as its definition does.
 */

#include "colonization.h"

typedef unsigned char *hp;

extern void handle_resize(short *hp, long size);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void report_error_1068_dc3e(void *ctx, char *msg);

long rll_decode(short *h)
{
    hp dend;
    hp send;
    hp src_base;
    hp dst_base;
    hp sp;
    hp dp;
    int run_byte;
    long cap;
    long size;
    long out;
    register int n;
    register int scratch;

    size = MSBlockSize(*h);
    scratch = MSAllocateHandle(size);
    if (scratch == 0) {
        report_error_1068_dc3e(&g_error_log, "RLLDecode could not allocate decompression buffer");
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
            run_byte = *sp;
            sp++;
            while (n-- != 0) {
                *dp = run_byte;
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
