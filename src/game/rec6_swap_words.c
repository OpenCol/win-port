/* rec6_swap_words: candidate for 1008:8bf7 (129 bytes, 3 callers).  Verdict: RELOCATABLE.
 *
 * The three words of four 6-byte records through `swap_word_identity` in place.
 */

#include "colonization.h"

#include "records.h"
/* same object: 4-byte `push cs; call ` (rule 47) */

void rec6_swap_words(Rec6 *r)
{
    int i;

    for (i = 0; i < 4; i++) {
        r[i].colony = swap_word_identity(r[i].colony);
        r[i].amount = swap_word_identity(r[i].amount);
        r[i].count = swap_word_identity(r[i].count);
    }
}
