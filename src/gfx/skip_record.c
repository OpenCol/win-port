/* PORT: the game's 16-bit words read and written as i16, not int (4 bytes here) (W4). */
/* skip_record: candidate for 1068:9921 (78 bytes).  Verdict: RELOCATABLE.
 *
 * Advances a pointer past one length-prefixed record: the length is the int at +2 and the header is four bytes.
 */

#include "colonization.h"

char * skip_record(char *p)
{
    return p + *(i16 *)(p + 2) + 4;
}
