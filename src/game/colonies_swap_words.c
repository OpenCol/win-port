/* colonies_swap_words: candidate for 1008:8751 (313 bytes, 3 callers).  Verdict: RELOCATABLE.
 *
 * Passes the word fields of n 202-byte records through `swap_word_identity` in place -- +0x90,
 * +0x92, +0x98 and sixteen at +0x9a -- and the two longs at +0xc2 and +0xc6 through the
 * 32-bit equivalent.
 */

#include "colonization.h"

#include "records.h"
/* same object: 4-byte `push cs; call ` (rule 47) */

void colonies_swap_words(Colony *colony, int colony_count)
{
    int j;
    int i;

    for (i = 0; i < colony_count; i++) {
        colony[i].changed = swap_word_identity(colony[i].changed);
        colony[i].hammers = swap_word_identity(colony[i].hammers);
        colony[i].bonus_hammers = swap_word_identity(colony[i].bonus_hammers);
        for (j = 0; j < 16; j++)
            colony[i].stock[j] = swap_word_identity(colony[i].stock[j]);
        colony[i].bells = identity_ptr_1008_d74e(colony[i].bells);
        colony[i].bells_cap = identity_ptr_1008_d74e(colony[i].bells_cap);
    }
}
