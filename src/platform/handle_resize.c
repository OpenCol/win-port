/* handle_resize -- shipped at 1070:d171.
 *
 * Ported by tools/port.py from matched/platform/handle_resize.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: handle -> v
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 MSAllocateHandle(col_i32 size);
col_i32 MSBlockSize(col_i16 h);
col_i32 min_long_1070_de2b(col_i32 a, col_i32 b);
void * MSHLock(col_i16 h);
void MSCopyMem(void *src, void *dst, col_i32 n);
void MSDisposeHandle(col_i16 h);
void MSHUnlock(col_i16 h);

/* same object: 4-byte `push cs; call ` */








void handle_resize(Rec *r, col_i32 size)
{
    col_i32 n;
    void *old;
    void *nw;
    register col_i16 h;

    h = MSAllocateHandle(size);
    old = MSHLock(r->v);
    nw = MSHLock(h);
    n = MSBlockSize(r->v);
    MSCopyMem(old, nw, min_long_1070_de2b(n, size));
    MSHUnlock(h);
    MSHUnlock(r->v);
    MSDisposeHandle(r->v);
    r->v = h;
}
