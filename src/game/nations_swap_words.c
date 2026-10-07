/* nations_swap_words: candidate for 1008:888a (816 bytes, 3 callers).  Verdict: RELOCATABLE.
 *
 * Byte-order pass over the four 316-byte nation records: eleven words and three longs
 * in the head of each record, then for sixteen slots a word at +0x5c and longs at +0x7c,
 * +0xbc and +0xfc. Two of the longs (+0x22, +0x26) go through the 16-bit swap_word_identity -- the
 * low word is pushed and the int result widened (CWDE) -- the rest through the 32-bit one.
 */

#include "colonization.h"

#include "records.h"
/* same object: 4-byte `push cs; call ` (rule 47) */

void nations_swap_words(Nation *nations)
{
    int j;
    int i;

    for (i = 0; i < 4; i++) {
        nations[i].bells = swap_word_identity(nations[i].bells);
        nations[i].bells_total = swap_word_identity(nations[i].bells_total);
        nations[i].last_tax_delta = swap_word_identity(nations[i].last_tax_delta);
        nations[i].next_father = swap_word_identity(nations[i].next_father);
        nations[i].f16 = swap_word_identity(nations[i].f16);
        nations[i].artillery = swap_word_identity(nations[i].artillery);
        nations[i].royal_gold = swap_word_identity(nations[i].royal_gold);
        nations[i].net_take = swap_word_identity(nations[i].net_take);
        nations[i].gold = identity_ptr_1008_d74e(nations[i].gold);
        nations[i].crosses = swap_word_identity(nations[i].crosses);
        nations[i].need = swap_word_identity(nations[i].need);
        nations[i].founded = swap_word_identity(nations[i].founded);
        nations[i].horses = swap_word_identity(nations[i].horses);
        nations[i].boycott = swap_word_identity(nations[i].boycott);
        for (j = 0; j < 16; j++) {
            nations[i].drift[j] = swap_word_identity(nations[i].drift[j]);
            nations[i].trade_gold[j] = identity_ptr_1008_d74e(nations[i].trade_gold[j]);
            nations[i].trade_tons[j] = identity_ptr_1008_d74e(nations[i].trade_tons[j]);
            nations[i].sold[j] = identity_ptr_1008_d74e(nations[i].sold[j]);
        }
    }
}
