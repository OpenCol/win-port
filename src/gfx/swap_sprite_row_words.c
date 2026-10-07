/* swap_sprite_row_words: candidate for 1068:67de (123 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Walks `n` variable-length records and puts the two `short`s at the front of each through
 * `swap_word_identity_1068_9518`, in place. `skip_record` is what finds the next one, so the
 * records are not a fixed stride and the cursor is the parameter itself.
 *
 * THE CURSOR IS A `` POINTER, and every access to the second `short` shows it: `MOV BX,2
 * / ADD BX,[bp+6] / ADC AX,0 / MOV CL,__AHSHIFT / SHL AX,CL / ADD AX,[bp+8] / MOV ES,AX` is
 * the normalisation Borland emits for `p[1]` on a pointer and for nothing else. A ``
 * pointer would be one `LES` and a displacement; `p[0]` IS one `LES`, because offset zero
 * needs no carry.
 *
 * That the same variable is addressed both ways in one function is the clearest statement of
 * what `` costs: nothing at offset 0, seven instructions at offset 2.
 *
 * RENAMED 2026-09-26 from remap_pairs: Only caller is Sprite_Init, after it loads a SPRT resource. The body walks the sprite's rows (n = the row count, with skip_record stepping over each variable-length run) and passes the two header words of each row, first opaque column and run length, through swap_word_identity. That is the Mac big-endian word swap, which compiles to an identity on the PC. So nothing is remapped: this is an endian fix-up of the row headers. In the sprite layer, 'remap' reads as a colour or value remapping, which the body does not do.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

void swap_sprite_row_words(short *p, int n)
{
    register int i;

    for (i = 0; i < n; i++) {
        p[0] = swap_word_identity_1068_9518(p[0]);
        p[1] = swap_word_identity_1068_9518(p[1]);
        p = (short *)skip_record((char *)p);
    }
}
