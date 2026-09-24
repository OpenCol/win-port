/* free_map_block -- shipped at 1040:76d5.
 *
 * Ported by tools/port.py from matched/game/free_map_block.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern MemBlock far g_text_pool[];                   SEG29:0x10dc
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void memblock_free(void *b);

typedef struct { char pad[2]; } MemBlock;



/* same object: 4-byte `push cs; call ` (rule 47) */


void free_map_block(void)
{
    memblock_free(g_text_pool);
}
