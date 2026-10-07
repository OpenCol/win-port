/* return_null: candidate for 1070:a89b (37 bytes).  Verdict: EXACT.
 *
 * Returns a null pointer whether or not the argument is null: a stub
 * left in the build.
 */

#include "colonization.h"

unsigned char *return_null(void *p)
{
    if (p == 0)
        return 0;
    return 0;
}
