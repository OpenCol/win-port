/* PORT: integer <-> pointer through uintptr_t/intptr_t: a resource id, a 16-bit handle token or a parameter block, the same bits on i386 and on 64-bit Windows (W9). */
/* block_size: candidate for 1078:0409 (29 bytes).  Verdict: RELOCATABLE.
 *
 * The size of the block behind a memory handle, forwarded to MSBlockSize in segment 15.
 *
 * Same shape as its neighbour `MSLockResource` (1078:0386): the body pushes one word, which
 * is the same instruction for an `int` and for the low half of a pointer, and the sole
 * caller (`text_open`) pushes a segment and an offset and cleans up with `ADD SP,4`.
 * So the argument is a pointer whose OFFSET half is the handle. The return is DX:AX --
 * `MSBlockSize` forwards `GlobalSize`, which is 32 bits.
 */

#include "colonization.h"

unsigned long block_size(void *resource)
{
    return MSBlockSize((int)(intptr_t)resource);
}
