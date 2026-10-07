/* rect_is_empty: candidate for 1038:9166 (62 bytes, 5 callers).  Verdict: RELOCATABLE.
 *
 * 1 when the rectangle's width or height is not positive.
 */

#include "colonization.h"

#include "w16types.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

int rect_is_empty(RECT *r)
{
    int empty = 0;

    if (rect_width_1038_dfbd(r) <= 0 || rect_height_1038_dfdc(r) <= 0)
        empty = 1;
    return empty;
}
