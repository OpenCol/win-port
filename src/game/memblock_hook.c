/* memblock_hook: candidate for 1040:a9f4 (14 bytes).  Verdict: EXACT.
 *
 * An empty hook called by heap_create and heap_lock.
 */

#include "colonization.h"

void memblock_hook(void *unused1, void *unused2, long unused3)
{
}
