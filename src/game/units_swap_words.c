/* units_swap_words -- shipped at 1008:8c78.
 *
 * Ported by tools/port.py from matched/game/units_swap_words.c.
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 swap_word_identity(col_i16 v);

/* same object: 4-byte `push cs; call ` (rule 47) */

void units_swap_words(Unit *u, col_i16 n)
{
    col_i16 i;

    for (i = 0; i < n; i++) {
        u[i].link = swap_word_identity(u[i].link);
        u[i].link2 = swap_word_identity(u[i].link2);
    }
}
