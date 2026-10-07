/* set_text_arg: candidate for 1050:920a (36 bytes).  Verdict: RELOCATABLE.
 *
 * Stores an int, sign-extended, into slot i of the long argument table the
 * TEXT formatter reads (SEG31:0x2d4).
 */

#include "colonization.h"

void set_text_arg(int i, int value)
{
    g_text_args[i] = value;
}
