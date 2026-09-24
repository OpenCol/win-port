/* memblock_alloc -- shipped at 1040:6a8a.
 *
 * Ported by tools/port.py from matched/game/memblock_alloc.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *   extern char g_fmt_alloc_failed[];                    DGROUP:0x1d95
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct {
    char kind;
    char flag;
    col_i16 handle;
    void *ptr;
    col_i16 f8;
    col_u16 size_a;
    col_u16 size_b;
} MemBlock;

extern col_u16 MSAllocateHandle(col_u32 size);
extern void memblock_hook(void *ctx, char *fmt, col_i32 value);



col_i16 memblock_alloc(MemBlock *b, col_i16 kind, col_u16 size)
{
    col_u16 n = size;
    col_i16 err = 1;

    b->kind = kind;
    b->handle = MSAllocateHandle(n);
    if (b->handle == 0)
        memblock_hook(&(*g_error_ctx), g_fmt_alloc_failed, (col_i32)kind);
    else {
        b->flag = 1;
        b->f8 = 0;
        b->size_b = n;
        b->size_a = n;
        b->ptr = 0;
        err = 0;
    }
    return err;
}
