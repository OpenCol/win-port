/* identity_ptr_1068_9625: candidate for 1068:9625 (24 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Returns its -pointer argument unchanged -- `MOV DX,[BP+8] / MOV AX,[BP+6]` and
 * nothing else. One of FOUR copies.
 *
 * 24 bytes, not the census 20: the written-out `return` adds a dead second epilogue
 * (rule 44).
 */

#include "colonization.h"

void *identity_ptr_1068_9625(void *p)
{
    return p;
}
