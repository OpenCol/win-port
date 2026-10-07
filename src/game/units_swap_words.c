/* units_swap_words: candidate for 1008:8c78 (100 bytes, 3 callers).  Verdict: RELOCATABLE.
 *
 * The words at +0x18 and +0x1a of n 28-byte unit records through `swap_word_identity` in place.
 */

#include "colonization.h"

#include "records.h"
/* same object: 4-byte `push cs; call ` (rule 47) */

void units_swap_words(Unit *u, int unit_count)
{
    int i;

    for (i = 0; i < unit_count; i++) {
        u[i].link = swap_word_identity(u[i].link);
        u[i].link2 = swap_word_identity(u[i].link2);
    }
}
