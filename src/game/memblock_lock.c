/* memblock_lock -- shipped at 1040:6b68.
 *
 * Ported by tools/port.py from matched/game/memblock_lock.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *   extern char g_fmt_lock_failed[];                     DGROUP:0x1da9
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void memblock_hook(void *ctx, char *fmt, col_i32 value);

typedef struct { unsigned char kind; char flag; col_i16 handle; void *ptr; } MemBlock;
extern void * MSHLock(col_i16 h);
/* same object: 4-byte `push cs; call ` (rule 47) */




col_i16 memblock_lock(MemBlock *b)
{
    col_i16 err = 1;

    if (b->ptr == 0) {
        b->ptr = MSHLock(b->handle);
        if (b->ptr == 0)
            memblock_hook(&(*g_error_ctx), g_fmt_lock_failed, (col_i32)b->kind);
    }
    err = 0;
    return err;
}
