/* handle_replace_with_copy -- shipped at 1070:cfea.
 *
 * Ported by tools/port.py from matched/platform/handle_replace_with_copy.c.
 *
 * The file's own Handle dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal GlobalUnlock(col_u16 h);
extern col_u32 pascal GlobalSize(col_u16 h);
extern void * pascal GlobalLock(col_u16 h);
extern void pascal hmemcpy(void *dst, void *src, col_u32 n);
#endif

extern col_u16 MSAllocateHandle(col_u32 size);

void handle_replace_with_copy(Handle *h)
{
    col_i32 n;
    void *src;
    void *dst;
    register col_u16 nh;

    n = GlobalSize(h->handle);
    nh = MSAllocateHandle(n);
    if (nh != 0) {
        dst = GlobalLock(nh);
        src = GlobalLock(h->handle);
        hmemcpy(dst, src, n);
        GlobalUnlock(nh);
        GlobalUnlock(h->handle);
        h->handle = nh;
    }
}
