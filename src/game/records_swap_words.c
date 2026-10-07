/* records_swap_words: candidate for 1008:864b (262 bytes, 3 callers).  Verdict: RELOCATABLE.
 *
 * Passes every word field of eight 78-byte records through `swap_word_identity` in place: two
 * words at +0xa and +0xc, sixteen at +0xe, and two runs of four at +0x2e and +0x46.
 */

#include "colonization.h"

#include "records.h"
/* same object: 4-byte `push cs; call ` (rule 47) */

void records_swap_words(Tribe *tribes)
{
    int i;
    int j;

    for (i = 0; i < 8; i++) {
        tribes[i].horses = swap_word_identity(tribes[i].horses);
        tribes[i].silver_base = swap_word_identity(tribes[i].silver_base);
        for (j = 0; j < 16; j++)
            tribes[i].goods_drift[j] = swap_word_identity(tribes[i].goods_drift[j]);
        for (j = 0; j < 4; j++) {
            tribes[i].mood[j] = swap_word_identity(tribes[i].mood[j]);
            tribes[i].anger[j] = swap_word_identity(tribes[i].anger[j]);
        }
    }
}
