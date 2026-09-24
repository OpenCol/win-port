/* colonies_swap_words -- shipped at 1008:8751.
 *
 * Ported by tools/port.py from matched/game/colonies_swap_words.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 swap_word_identity(col_i16 v);
col_i32 identity_ptr_1008_d74e(col_i32 v);

typedef struct {
    char pad0[0x90];
    col_i16 f90;
    col_i16 f92;
    char pad1[4];
    col_i16 f98;
    col_i16 f9a[16];
    char pad2[0xc2 - 0xba];
    col_i32 fc2;
    col_i32 fc6;
} Rec_local;
/* same object: 4-byte `push cs; call ` (rule 47) */


void colonies_swap_words(Rec_local *r, col_i16 n)
{
    col_i16 j;
    col_i16 i;

    for (i = 0; i < n; i++) {
        r[i].f90 = swap_word_identity(r[i].f90);
        r[i].f92 = swap_word_identity(r[i].f92);
        r[i].f98 = swap_word_identity(r[i].f98);
        for (j = 0; j < 16; j++)
            r[i].f9a[j] = swap_word_identity(r[i].f9a[j]);
        r[i].fc2 = identity_ptr_1008_d74e(r[i].fc2);
        r[i].fc6 = identity_ptr_1008_d74e(r[i].fc6);
    }
}
