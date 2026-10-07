/* identity_ptr_1068_9991: candidate for 1068:9991 (24 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Returns its -pointer argument unchanged -- `MOV DX,[BP+8] / MOV AX,[BP+6]` and
 * nothing else. One of FOUR copies.
 *
 * 24 bytes, not the census 20: the written-out `return` adds a dead second epilogue
 * (rule 44).
 */

#include "colonization.h"

long identity_ptr_1068_9991(long p)
{
    return p;
}
