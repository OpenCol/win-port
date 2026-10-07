/* skip_record: candidate for 1068:9921 (78 bytes).  Verdict: RELOCATABLE.
 *
 * Advances a pointer past one length-prefixed record: the length is the int at +2 and the header is four bytes.
 */

#include "colonization.h"

char * skip_record(char *p)
{
    return p + *(int *)(p + 2) + 4;
}
