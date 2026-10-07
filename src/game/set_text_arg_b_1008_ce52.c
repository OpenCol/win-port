/* set_text_arg_b_1008_ce52: candidate for 1008:ce52 (36 bytes).  Verdict: RELOCATABLE.
 *
 * Stores an unsigned char, zero-extended, into slot i of the long argument table the TEXT formatter
 * reads (SEG31:0x2d4).  Segment 1008's copy: the parameter's type is the whole
 * difference between the four variants, and it is visible in the widening --
 * `MOVSX`/`MOVZX`, word or byte.
 */

#include "colonization.h"

void set_text_arg_b_1008_ce52(int i, unsigned char value)
{
    g_text_args[i] = value;
}
