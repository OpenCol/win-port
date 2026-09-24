/* rec6_swap_words -- shipped at 1008:8bf7.
 *
 * Ported by tools/port.py from matched/game/rec6_swap_words.c.
 *
 * The file's own Rec6 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 swap_word_identity(col_i16 v);

/* same object: 4-byte `push cs; call ` (rule 47) */

void rec6_swap_words(Rec6 *r)
{
    col_i16 i;

    for (i = 0; i < 4; i++) {
        r[i].a = swap_word_identity(r[i].a);
        r[i].amount = swap_word_identity(r[i].amount);
        r[i].count = swap_word_identity(r[i].count);
    }
}
