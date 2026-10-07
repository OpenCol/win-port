/* list_current_value: candidate for 1068:9e6d (57 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 57, not the census 51. Same six bytes, same reason.
 *
 * The value under the cursor, without moving it. The twin of `list_first_value`, which
 * rewinds first, and the tail of `VList_Next`, which advances first.
 *
 * The `else` is load-bearing: with it Borland jumps past the alternative and emits a
 * third, unreachable epilogue at the end of the function, which is what the shipped
 * `EB 08` and the trailing `1F 5D 4D CB` are. Without it there are two epilogues and
 * the function is six bytes shorter.
 */

#include "colonization.h"

#include "records.h"
/* same object: 4-byte `push cs; call ` */

long list_current_value(ListIter *it)
{
    if (it->cur != 0)
        return deref_long(it->cur);
    else
        return 0L;
}
