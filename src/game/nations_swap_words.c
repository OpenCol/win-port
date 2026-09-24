/* nations_swap_words -- shipped at 1008:888a.
 *
 * Ported by tools/port.py from matched/game/nations_swap_words.c.
 *
 * The file's own Nation dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: horses -> horses_stock, royal_gold -> f22
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 swap_word_identity(col_i16 v);
col_i32 identity_ptr_1008_d74e(col_i32 v);

/* same object: 4-byte `push cs; call ` (rule 47) */


void nations_swap_words(Nation *r)
{
    col_i16 j;
    col_i16 i;

    for (i = 0; i < 4; i++) {
        r[i].bells = swap_word_identity(r[i].bells);
        r[i].bells_total = swap_word_identity(r[i].bells_total);
        r[i].f10 = swap_word_identity(r[i].f10);
        r[i].next_father = swap_word_identity(r[i].next_father);
        r[i].f16 = swap_word_identity(r[i].f16);
        r[i].artillery = swap_word_identity(r[i].artillery);
        r[i].f22 = swap_word_identity(r[i].f22);
        r[i].f26 = swap_word_identity(r[i].f26);
        r[i].gold = identity_ptr_1008_d74e(r[i].gold);
        r[i].crosses = swap_word_identity(r[i].crosses);
        r[i].need = swap_word_identity(r[i].need);
        r[i].founded = swap_word_identity(r[i].founded);
        r[i].horses_stock = swap_word_identity(r[i].horses_stock);
        r[i].boycott = swap_word_identity(r[i].boycott);
        for (j = 0; j < 16; j++) {
            ((col_i16 *)((unsigned char *)&(r[i]) + 0x5c))[j] = swap_word_identity(((col_i16 *)((unsigned char *)&(r[i]) + 0x5c))[j]);
            ((col_i32 *)((unsigned char *)&(r[i]) + 0x7c))[j] = identity_ptr_1008_d74e(((col_i32 *)((unsigned char *)&(r[i]) + 0x7c))[j]);
            ((col_i32 *)((unsigned char *)&(r[i]) + 0xbc))[j] = identity_ptr_1008_d74e(((col_i32 *)((unsigned char *)&(r[i]) + 0xbc))[j]);
            ((col_i32 *)((unsigned char *)&(r[i]) + 0xfc))[j] = identity_ptr_1008_d74e(((col_i32 *)((unsigned char *)&(r[i]) + 0xfc))[j]);
        }
    }
}
