/* init_record: candidate for 1040:6b19 (79 bytes).  Verdict: EXACT.
 *
 * Fills a 14-byte record: byte +1 cleared, +0 = a, +2 = b, the long at +4
 * = c, +8 cleared, +0xa and +0xc = d.
 */

#include "colonization.h"

#include "records.h"

void init_record(MemBlock *r, int kind, int handle, long ptr, int size)
{
    r->owns_handle = 0;
    r->kind = kind;
    r->handle = handle;
    r->ptr = (void *)ptr;
    r->used = 0;
    r->size = r->avail = size;
}
