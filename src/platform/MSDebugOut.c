/* MSDebugOut: candidate for 1070:848a (41 bytes).  Verdict: RELOCATABLE.
 *
 * OutputDebugString(s) followed by a newline; returns 1.
 */

#include "colonization.h"

int MSDebugOut(const char *s)
{
    OutputDebugString(s);
    OutputDebugString("\n");
    return 1;
}
