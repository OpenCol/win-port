/* set_text_arg_i_1010_ebb0: candidate for 1010:ebb0 (36 bytes).  Verdict: RELOCATABLE.
 *
 * Stores an int, sign-extended, into slot i of the long argument table the
 * TEXT formatter reads (SEG31:0x2d4).
 *
 * RENAMED 2026-09-26 from set_text_arg_1010_ebb0: byte- and source-identical to the seven set_text_arg_i copies: g_text_args[i] = v for an int v.
 */

#include "colonization.h"

void set_text_arg_i_1010_ebb0(int i, int value)
{
    g_text_args[i] = value;
}
