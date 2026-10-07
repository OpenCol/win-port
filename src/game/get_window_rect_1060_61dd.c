/* get_window_rect_1060_61dd: candidate for 1060:61dd (38 bytes).  Verdict: RELOCATABLE.
 *
 * Copies the window's rectangle at +0x76a into *r.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void get_window_rect_1060_61dd(ColChild *w, RECT *r)
{
    *r = w->rect;
}
