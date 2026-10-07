/* rle_pack_handle: candidate for 1068:c831 (828 bytes).  Verdict: RELOCATABLE.
 * size-source: 828, not the census 820. The trailing 8 bytes are the dead second epilogue.
 *
 * Run-length packs the first `n` bytes of a memory handle in place and returns the packed length;
 * `rll_decode` directly above it is the unpacker.
 * A scratch handle is allocated at n + n/128 + 5, the worst case. The input is read as
 * alternating runs:
 *
 *   a byte repeated three or more times  ->  count (1..0x7f), then the byte
 *   anything else, up to the next repeat ->  0x80 + count (1..0x7f), then that many bytes
 *
 * each split into pieces of at most 0x7f. A repeat of exactly two is left in the literal stretch.
 * Then the caller's handle is freed, reallocated at the packed size, filled from the scratch copy,
 * and the scratch handle freed.
 *
 * Every pointer is ``, so each step is an `__AHSHIFT` normalisation, and each length is a
 * -pointer difference -- the `SHR BX,3 / SHR AX,3 / SUB / SBB` pair that turns two selectors
 * into a 32-bit count.
 *
 * THE COMPILER GETS `p[1]` WRONG, and the shipped game has the defect. For the literal scan's
 * `p[1] != *p` it normalises p + 1 into AX:BX, never moves AX into ES, and then loads the byte
 * into AL -- so the read goes through whatever segment ES last held. Straight after a run was
 * written that is the OUTPUT buffer's segment, a different handle; on later passes it is p's own,
 * right only while p + 1 shares it. This source reproduces it byte for byte; no spelling was
 * needed to provoke it.
 */

#include "colonization.h"

typedef unsigned char *hp;

long rle_pack_handle(short *h, long n)
{
    hp start;
    hp out;
    hp src;
    hp lit;
    hp p;
    hp end;
    long total;
    long cnt;
    long run;
    register short scratch;

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
