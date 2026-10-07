/* MSAllocateHandle_1068_93e9: candidate for 1068:93e9 (30 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's one-line forwarder to `MSAllocateHandle`, named for the routine it reaches.
 * Its first 26 bytes are `random_set_seed`'s whole body except for the one relocated call word, which the
 * comparator masks, and the 4 after them are the dead second epilogue random_set_seed lacks -- so the callee's name comes from the disassembly and not from the
 * verdict.
 *
 * 30 bytes, not the census 26: the written-out `return` adds a dead second epilogue
 * (rule 44).
 */

#include "colonization.h"

short MSAllocateHandle_1068_93e9(long size)
{
    return MSAllocateHandle(size);
}
