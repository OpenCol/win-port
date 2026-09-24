/* CTable_load_rgb -- shipped at 1068:0a5b.
 *
 * Ported by tools/port.py from matched/gfx/CTable_load_rgb.c.
 *
 * The file's own CTable dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSHLock(col_i16 h);
extern void * MSHUnlock(col_i16 h);
extern void MSCopyMem(void *src, void *dst, col_i32 n);

void CTable_load_rgb(CTable *t, col_i16 count, void *src)
{
    col_i16 n = count;
    void *p;

    p = MSHLock(t->handle);
    MSCopyMem(src, p, (col_i32)(n * 3));
    p = MSHUnlock(t->handle);
}
