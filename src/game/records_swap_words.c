/* records_swap_words -- shipped at 1008:864b.
 *
 * Ported by tools/port.py from matched/game/records_swap_words.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 swap_word_identity(col_i16 v);

typedef struct {
    char pad0[0xa];
    col_i16 fa;
    col_i16 right;
    col_i16 fe[16];
    col_i16 f2e[4];
    char pad1[0x46 - 0x36];
    col_i16 f46[4];
} Rec_local;
/* same object: 4-byte `push cs; call ` (rule 47) */

void records_swap_words(Rec_local *r)
{
    col_i16 i;
    col_i16 j;

    for (i = 0; i < 8; i++) {
        r[i].fa = swap_word_identity(r[i].fa);
        r[i].right = swap_word_identity(r[i].right);
        for (j = 0; j < 16; j++)
            r[i].fe[j] = swap_word_identity(r[i].fe[j]);
        for (j = 0; j < 4; j++) {
            r[i].f2e[j] = swap_word_identity(r[i].f2e[j]);
            r[i].f46[j] = swap_word_identity(r[i].f46[j]);
        }
    }
}
