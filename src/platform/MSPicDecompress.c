/* MSPicDecompress -- shipped at 1078:0000.
 *
 * Ported by tools/port.py from matched/platform/MSPicDecompress.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void stub_reporter(void *ctx, char *msg);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern col_u16 MSAllocateHandle(col_u32 size);
extern col_u16 * MSHLock(col_i16 h);
extern void MSHUnlock(col_i16 h);
extern col_i16 MSDisposeHandle(col_u16 h);
extern void sub_1088_0000(col_i32 src, col_i32 f, col_i32 tables, void *e, col_i16 d, col_i16 c);

col_i16 MSPicDecompress(col_i32 src, unsigned char base, col_i16 c, col_i16 d, void *e, col_i32 f)
{
    col_u16 *t0;
    col_u16 *t1;
    col_u16 *t2;
    col_u16 h;
    col_i16 i;

    h = MSAllocateHandle(0x6000L);
    if (h == 0) {
        stub_reporter(&(*g_error_ctx), "Error: Cannot allocate decompression tables in MSPicDecompress");
        return 0;
    }
    t0 = MSHLock(h);
    t1 = (col_u16 *)((char *)t0 + 0x2000);
    t2 = (col_u16 *)((char *)t0 + 0x4000);
    for (i = 0; i < 0x1000; i++) {
        t0[i] = 0xffff;
        t1[i] = base + i;
        t2[i] = 1;
    }
    sub_1088_0000(src, f, (col_i32)t0, e, d, c);
    MSHUnlock(h);
    MSDisposeHandle(h);
    return 1;
}
