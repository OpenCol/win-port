/* MSBlockSize: candidate for 1070:d157 (26 bytes).  Verdict: RELOCATABLE.
 *
 * GlobalSize(h) -- the allocated size of a global handle.
 */

#include "colonization.h"

long MSBlockSize(int handle)
{
    return GlobalSize(handle);
}
