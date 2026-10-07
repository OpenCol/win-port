/* set_text_arg_i_1038_86cc: candidate for 1038:86cc (36 bytes).  Verdict: RELOCATABLE.
 *
 * Stores an int, sign-extended, into slot i of the long argument table the TEXT formatter
 * reads (SEG31:0x2d4).  Segment 1038's copy: the parameter's type is the whole
 * difference between the four variants, and it is visible in the widening --
 * `MOVSX`/`MOVZX`, word or byte.
 */

#include "colonization.h"

void set_text_arg_i_1038_86cc(int i, int value)
{
    g_text_args[i] = value;
}
