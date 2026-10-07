/* MSCopyMem: candidate for 1070:d134 (35 bytes).  Verdict: RELOCATABLE.
 *
 * hmemcpy(dst, src, n) with the first two arguments SWAPPED: this takes (src, dst, n) and
 * the Windows call takes (dst, src, n). That is the Macintosh BlockMove order, which is
 * what the whole MS* layer is shaped like.
 *
 * The order is not a guess. hmemcpy is PASCAL, so arguments are pushed LEFT TO RIGHT, and
 * the shipped pushes are [bp+0xc],[bp+0xa] -- a pointer whose offset is the SECOND
 * parameter -- then [bp+0x8],[bp+0x6], then the dword at [bp+0xe]. First pushed is first
 * argument, so the second parameter is the destination.
 */

#include "colonization.h"

void MSCopyMem(void *src, void *dst, long n)
{
    hmemcpy(dst, src, n);
}
