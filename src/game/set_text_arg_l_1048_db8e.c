/* set_text_arg_l_1048_db8e: candidate for 1048:db8e (35 bytes).  Verdict: RELOCATABLE.
 *
 * Stores a long into slot i of the long argument table the TEXT formatter
 * reads (SEG31:0x2d4).  Segment 1048's copy of the four-byte variant: no
 * widening, because the parameter is already the table's width.
 */

#include "colonization.h"

void set_text_arg_l_1048_db8e(int i, long value)
{
    g_text_args[i] = value;
}
