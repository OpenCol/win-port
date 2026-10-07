/* set_text_arg_u_1030_eafc: candidate for 1030:eafc (36 bytes).  Verdict: RELOCATABLE.
 *
 * Stores an unsigned int, zero-extended, into slot i of the long argument table the TEXT formatter
 * reads (SEG31:0x2d4).  Segment 1030's copy: the parameter's type is the whole
 * difference between the four variants, and it is visible in the widening --
 * `MOVSX`/`MOVZX`, word or byte.
 */

#include "colonization.h"

void set_text_arg_u_1030_eafc(int i, unsigned value)
{
    g_text_args[i] = value;
}
