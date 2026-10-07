/* set_text_arg_u_1028_e3ca: candidate for 1028:e3ca (36 bytes).  Verdict: RELOCATABLE.
 *
 * Stores an unsigned int, zero-extended, into slot i of the long argument table the TEXT formatter
 * reads (SEG31:0x2d4).  Segment 1028's copy: the parameter's type is the whole
 * difference between the four variants, and it is visible in the widening --
 * `MOVSX`/`MOVZX`, word or byte.
 */

#include "colonization.h"

void set_text_arg_u_1028_e3ca(int i, unsigned arg)
{
    g_text_args[i] = arg;
}
