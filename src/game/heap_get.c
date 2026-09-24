/* heap_get -- shipped at 1040:6c52.
 *
 * Ported by tools/port.py from matched/game/heap_get.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 memblock_lock(void *h);
void empty_1040_aa02(void *ctx, char *msg, col_i16 id, col_u16 n);

typedef struct {
    unsigned char id; /* +0x00 reported in the "No more heap" message */
    char pad1[3];
    char *base; /* +0x04 offset, +0x06 segment */
    col_u16 used; /* +0x08 */
    char pad2[2];
    col_u16 avail; /* +0x0c */
} Heap;


/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR. */

   /* the Macintosh heap_lock(Heap*) */

void * heap_get(Heap *h, col_u16 n)
{
    char *p;
    char *q;

    p = 0;
    if (h->avail < n)
        empty_1040_aa02(&(*g_error_ctx), "No more heap", h->id, n);
    if (memblock_lock(h) != 0)
        goto done;
    q = h->base + h->used;
    p = q;
    h->used += n;
    h->avail -= n;
done:
    return p;
}
