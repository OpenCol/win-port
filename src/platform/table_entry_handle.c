/* table_entry_handle: candidate for 1078:12c7 (133 bytes).  Verdict: RELOCATABLE.
 *
 * Wraps entry i of the table inside handle h (count at +0x100, entries from +0x106) in
 * a fresh 4-byte handle: the entry's word then a zero word. Returns the new handle, or
 * 0 when i is out of range. The source table is locked and never unlocked.
 */

#include "colonization.h"

typedef struct { short value; short zero; } Entry;

short table_entry_handle(short h, int i)
{
    int n = i;
    short handle = 0;
    Table *t;
    Entry *e;

    t = (Table *)MSHLock(h);
    if (t->count > n) {
        handle = MSAllocateHandle(4L);
        e = (Entry *)MSHLock(handle);
        e->value = t->entries[n];
        e->zero = 0;
        MSHUnlock(handle);
    }
    return handle;
}
