/* set_text_arg_i_1028_e3ee: candidate for 1028:e3ee (36 bytes).  Verdict: RELOCATABLE.
 *
 * Stores an int, sign-extended, into slot i of the long argument table the TEXT formatter
 * reads (SEG31:0x2d4).  Segment 1028's copy: the parameter's type is the whole
 * difference between the four variants, and it is visible in the widening --
 * `MOVSX`/`MOVZX`, word or byte.
 */

#include "colonization.h"

void set_text_arg_i_1028_e3ee(int i, int value)
{
    g_text_args[i] = value;
}
