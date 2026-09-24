/* remap_pairs -- shipped at 1068:67de.
 *
 * Ported by tools/port.py from matched/gfx/remap_pairs.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 swap_word_identity_1068_9518(col_i16 v);
void * skip_record(void *p);

/* same object: 4-byte `push cs; call ` (rule 47) */


void remap_pairs(col_i16 *p, col_i16 n)
{
    register col_i16 i;

    for (i = 0; i < n; i++) {
        p[0] = swap_word_identity_1068_9518(p[0]);
        p[1] = swap_word_identity_1068_9518(p[1]);
        p = skip_record(p);
    }
}
