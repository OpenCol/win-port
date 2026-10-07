/* set_text_arg_i_1048_db6a: candidate for 1048:db6a (36 bytes).  Verdict: RELOCATABLE.
 *
 * Stores an int, sign-extended, into slot i of the long argument table the
 * TEXT formatter reads (SEG31:0x2d4).
 *
 * RENAMED 2026-09-26 from dialog_set_option_value: byte-identical to set_text_arg_i_1010_ebb0/set_text_arg_i_1008_cda6/set_text_arg_i_1018_b472/set_text_arg_i_1020_a926/set_text_arg_i_1028_e3ee/set_text_arg_i_1030_eb46/set_text_arg_i_1038_86cc (analysis/re-exact-dupes.json)/so it takes that family's name.
 */

#include "colonization.h"

void set_text_arg_i_1048_db6a(int i, int value)
{
    g_text_args[i] = value;
}
