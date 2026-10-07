/* MSCopyMem_1068_9424: candidate for 1068:9424 (38 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `save_read_field`'s twin reaching `MSCopyMem` instead of `MSReadFile`: the same three
 * arguments, a pointer each and a `long`, passed straight through.
 *
 * FOUR bytes shorter than the twin, and that is the whole difference in the source: this
 * one has no written-out `return`, so it has one epilogue where the twin has two (rule 44).
 * The comparator masks the relocated call word, so the callee's name comes from the
 * disassembly and not from the verdict.
 *
 * The order is (src, dst, n), the Macintosh BlockMove order -- proved in `MSCopyMem`'s own
 * file, where the PASCAL `hmemcpy` it forwards to takes the SECOND parameter as the
 * destination. A pass-through forwarder cannot show it, so this file just follows.
 */

#include "colonization.h"

void MSCopyMem_1068_9424(void *src, void *dst, long n)
{
    MSCopyMem(src, dst, n);
}
