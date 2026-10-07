/* set_text_arg_char_1020_a973: candidate for 1020:a973 (36 bytes).  Verdict: RELOCATABLE.
 *
 * Stores a char, sign-extended, into slot i of the long argument table the TEXT formatter
 * reads (SEG31:0x2d4).  Segment 1020's copy: the parameter's type is the whole
 * difference between the four variants, and it is visible in the widening --
 * `MOVSX`/`MOVZX`, word or byte.
 *
 * RENAMED 2026-09-26 from set_text_arg_c: byte-identical to set_text_arg_char (analysis/re-exact-dupes.json)/so it takes that family's name.
 */

#include "colonization.h"

void set_text_arg_char_1020_a973(int i, char value)
{
    g_text_args[i] = (long)value;
}
