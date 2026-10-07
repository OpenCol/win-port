/* rect_height_1078_3418: candidate for 1078:3418 (32 bytes).  Verdict: EXACT.
 *
 * The height of a rectangle, bottom minus top.
 */

#include "colonization.h"

#include "w16types.h"
int rect_height_1078_3418(RECT *r) { return r->bottom - r->top; }
