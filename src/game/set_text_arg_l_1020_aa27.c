/* set_text_arg_l_1020_aa27: candidate for 1020:aa27 (35 bytes).  Verdict: RELOCATABLE.
 *
 * Stores a long into slot i of the long argument table the TEXT formatter
 * reads (SEG31:0x2d4).  Segment 1020's copy of the four-byte variant: no
 * widening, because the parameter is already the table's width.
 */

#include "colonization.h"

void set_text_arg_l_1020_aa27(int i, long arg)
{
    g_text_args[i] = arg;
}
