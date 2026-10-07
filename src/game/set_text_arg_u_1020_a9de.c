/* set_text_arg_u_1020_a9de: candidate for 1020:a9de (36 bytes).  Verdict: RELOCATABLE.
 *
 * Stores an unsigned int, zero-extended, into slot i of the long argument table the TEXT formatter
 * reads (SEG31:0x2d4).  Segment 1020's copy: the parameter's type is the whole
 * difference between the four variants, and it is visible in the widening --
 * `MOVSX`/`MOVZX`, word or byte.
 */

#include "colonization.h"

void set_text_arg_u_1020_a9de(int i, unsigned value)
{
    g_text_args[i] = value;
}
