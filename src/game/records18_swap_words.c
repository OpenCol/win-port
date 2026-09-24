/* records18_swap_words -- shipped at 1008:85f6.
 *
 * Ported by tools/port.py from matched/game/records18_swap_words.c.
 *
 * The file's own Rec18 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 swap_word_identity(col_i16 v);

/* same object: 4-byte `push cs; call ` (rule 47) */

void records18_swap_words(Rec18 *r, col_i16 n)
{
    col_i16 i;
    col_i16 j;

    for (i = 0; i < n; i++)
        for (j = 0; j < 4; j++)
            ((col_i16 *)((unsigned char *)&(r[i]) + 0xa))[j] = swap_word_identity(((col_i16 *)((unsigned char *)&(r[i]) + 0xa))[j]);
}
