/* to_upper_char: candidate for 1040:5c70 (39 bytes).  Verdict: EXACT.
 *
 * Upper-cases one character, the byte-wide sibling of `to_upper`.
 * The range test is `JC`/`JA`, unsigned, so the parameter is an unsigned char
 * rather than an int.
 */

#include "colonization.h"

unsigned char to_upper_char(unsigned char c)
{
    if (c >= 'a' && c <= 'z')
        c -= 0x20;
    return c;
}
