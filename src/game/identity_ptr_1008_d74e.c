/* identity_ptr_1008_d74e: candidate for 1008:d74e (24 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Returns its long argument unchanged (every caller passes a long: the Mac build's byte swap) -- `MOV DX,[BP+8] / MOV AX,[BP+6]` and
 * nothing else. One of FOUR copies.
 *
 * 24 bytes, not the census 20: the written-out `return` adds a dead second epilogue
 * (rule 44).
 */

#include "colonization.h"

long identity_ptr_1008_d74e(long p)
{
    return p;
}
