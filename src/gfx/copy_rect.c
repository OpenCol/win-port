/* copy_rect: candidate for 1068:996f (34 bytes).  Verdict: RELOCATABLE.
 *
 * An eight-byte structure copy between two pointers, the graphics
 * library's rectangle assignment.
 */

#include "colonization.h"

#include "w16types.h"
void copy_rect(RECT *src, RECT *dst)
{
    *dst = *src;
}
