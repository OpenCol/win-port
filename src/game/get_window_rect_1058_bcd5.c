/* get_window_rect_1058_bcd5: candidate for 1058:bcd5 (38 bytes).  Verdict: RELOCATABLE.
 *
 * The report module's copy of get_window_rect: the window's rectangle at
 * +0x76a copied out through F_SCOPY.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void get_window_rect_1058_bcd5(ColChild *w, RECT *r)
{
    *r = w->rect;
}
