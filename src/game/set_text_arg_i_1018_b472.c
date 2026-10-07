/* set_text_arg_i_1018_b472: candidate for 1018:b472 (36 bytes).  Verdict: RELOCATABLE.
 *
 * Stores an int, sign-extended, into slot i of the long argument table the TEXT formatter
 * reads (SEG31:0x2d4).  Segment 1018's copy: the parameter's type is the whole
 * difference between the four variants, and it is visible in the widening --
 * `MOVSX`/`MOVZX`, word or byte.
 */

#include "colonization.h"

void set_text_arg_i_1018_b472(int i, int arg)
{
    g_text_args[i] = arg;
}
