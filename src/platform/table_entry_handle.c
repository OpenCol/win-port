/* table_entry_handle -- shipped at 1078:12c7.
 *
 * Ported by tools/port.py from matched/platform/table_entry_handle.c.
 *
 * The file's own Table, Entry dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: value -> name, zero -> kind
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_u16 MSAllocateHandle(col_u32 size);
extern void * MSHLock(col_i16 h);
extern void * MSHUnlock(col_i16 h);

col_i16 table_entry_handle(col_i16 h, col_i16 i)
{
    col_i16 n = i;
    col_i16 handle = 0;
    Table *t;
    Entry *e;

    t = (Table *)MSHLock(h);
    if (t->count > n) {
        handle = MSAllocateHandle(4L);
        e = (Entry *)MSHLock(handle);
        e->name = ((col_i16 *)((unsigned char *)(t) + 0x106))[n];
        e->kind = 0;
        MSHUnlock(handle);
    }
    return handle;
}
