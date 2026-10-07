/* crc: candidate for 1040:5e57 (40 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * One Galois LFSR step on *p with tap mask k: shift right, xor k when the
 * bit shifted out was set.  Returns the new value (the word is not stored).
 *
 * RENAMED 2026-09-26 from lfsr_step: the Mac build's crc(short*, short), bound by call-sequence alignment (1.0 aligned gaps in fill_docks leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 2.30x this one. A CRC is built from exactly this Galois LFSR step (the Mac body is the larger).
 */

#include "colonization.h"

int crc(int *p, int taps)
{
    int state = *p;

    if (state & 1) {
        state >>= 1;
        state ^= taps;
    } else {
        state >>= 1;
    }
    return state;
}
