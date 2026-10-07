/* rect_width_1078_33f9: candidate for 1078:33f9 (31 bytes).  Verdict: EXACT.
 *
 * The width of a rectangle, right minus left.
 */

#include "colonization.h"

#include "w16types.h"
int rect_width_1078_33f9(RECT *r) { return r->right - r->left; }
