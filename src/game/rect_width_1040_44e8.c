/* rect_width_1040_44e8: candidate for 1040:44e8 (31 bytes).  Verdict: EXACT.
 *
 * right - left of a RECT.
 */

#include "colonization.h"

#include "w16types.h"
int rect_width_1040_44e8(RECT *r)
{
    return r->right - r->left;
}
