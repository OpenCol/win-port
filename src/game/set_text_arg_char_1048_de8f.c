/* set_text_arg_char_1048_de8f: candidate for 1048:de8f (36 bytes).  Verdict: RELOCATABLE.
 *
 * Stores a char, sign-extended, into slot i of the long argument table the
 * TEXT formatter reads (SEG31:0x2d4).
 *
 * RENAMED 2026-09-26 from set_text_arg_char: byte-identical to set_text_arg_c (analysis/re-exact-dupes.json)/so it takes that family's name.
 */

#include "colonization.h"

void set_text_arg_char_1048_de8f(int i, char value)
{
    g_text_args[i] = (long)value;
}
