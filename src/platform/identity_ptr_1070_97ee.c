/* identity_ptr_1070_97ee: candidate for 1070:97ee (24 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Returns its -pointer argument unchanged -- `MOV DX,[BP+8] / MOV AX,[BP+6]` and
 * nothing else. One of FOUR copies.
 *
 * 24 bytes, not the census 20: the written-out `return` adds a dead second epilogue
 * (rule 44).
 */

#include "colonization.h"

#include "records.h"
Palette *identity_ptr_1070_97ee(void *p)
{
    return (Palette *)p;
}
