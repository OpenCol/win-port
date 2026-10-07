/* set_text_arg_l_1030_ec2f: candidate for 1030:ec2f (35 bytes).  Verdict: RELOCATABLE.
 *
 * Stores a long into slot i of the long argument table the TEXT formatter
 * reads (SEG31:0x2d4).  Segment 1030's copy of the four-byte variant: no
 * widening, because the parameter is already the table's width.
 */

#include "colonization.h"

void set_text_arg_l_1030_ec2f(int i, long value)
{
    g_text_args[i] = value;
}
