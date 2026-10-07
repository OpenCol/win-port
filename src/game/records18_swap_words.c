/* records18_swap_words: candidate for 1008:85f6 (85 bytes, 3 callers).  Verdict: RELOCATABLE.
 *
 * Four words at +0xa of each of n 18-byte records through `swap_word_identity` in place.
 */

#include "colonization.h"

#include "records.h"
/* same object: 4-byte `push cs; call ` (rule 47) */

void records18_swap_words(Settlement *settlements, int settlement_count)
{
    int i;
    int j;

    for (i = 0; i < settlement_count; i++)
        for (j = 0; j < 4; j++)
            settlements[i].anger[j] = swap_word_identity(settlements[i].anger[j]);
}
