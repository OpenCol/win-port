/* memblock_free -- shipped at 1040:6c03.
 *
 * Ported by tools/port.py from matched/game/memblock_free.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void resource_unlock(void *b);

typedef struct { char kind; char flag; col_i16 handle; void *ptr; col_i16 f8; col_u16 size_a; col_u16 size_b; } MemBlock;
/* same object: 4-byte `push cs; call ` (rule 47) */

extern col_i16 MSDisposeHandle(col_u16 h);

void memblock_free(MemBlock *b)
{
    if (b->flag != 0) {
        resource_unlock(b);
        MSDisposeHandle(b->handle);
    }
    b->f8 = 0;
    b->size_b = 0;
    b->size_a = 0;
}
