/* to_upper: candidate for 1040:5c97 (36 bytes).  Verdict: EXACT.
 *
 * Upper-cases one character, the module's own copy rather than the runtime's.
 */

#include "colonization.h"

int to_upper(int ch)
{
    if (ch >= 'a' && ch <= 'z')
        ch -= 0x20;
    return ch;
}
