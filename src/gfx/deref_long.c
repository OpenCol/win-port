/* deref_long: candidate for 1068:d5fe (28 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 28, not the census 24. The last 4 bytes are an unreachable epilogue;
 * Ghidra sizes by reachable code.
 *
 * Reads the long a pointer points at. The list accessors in this segment --
 * `list_first_value`, `list_current_value`, `VList_Next` -- all go through it rather than
 * dereferencing inline, which is why every one of them ends in a call.
 */

#include "colonization.h"

long deref_long(long *item)
{
    return *item;
}
