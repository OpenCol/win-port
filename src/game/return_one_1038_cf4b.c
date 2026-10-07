/* return_one_1038_cf4b: candidate for 1038:cf4b (35 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Returns 1 through a LOCAL, not directly: `SUB SP,2 / MOV WORD [BP-4],1 /
 * MOV AX,[BP-4]`. A bare `return 1;` materialises the constant in AX and costs no
 * frame at all, so the local is in the source and the function once did more with it.
 * One of TWO copies.
 *
 * 35 bytes, not the census 28: the written-out `return` adds a dead second epilogue
 * (rule 44).
 */

#include "colonization.h"

int return_one_1038_cf4b(void)
{
    int r = 1;

    return r;
}
