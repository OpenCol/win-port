/* nation52_swap_word_at_32 -- shipped at 1008:8bba.
 *
 * Ported by tools/port.py from matched/game/nation52_swap_word_at_32.c.
 *
 * The file's own Rec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 swap_word_identity(col_i16 v);

/* same object: 4-byte `push cs; call ` (rule 47) */

void nation52_swap_word_at_32(Rec52 *r)
{
    col_i16 i;

    for (i = 0; i < 4; i++)
        r[i].f32 = swap_word_identity(r[i].f32);
}
