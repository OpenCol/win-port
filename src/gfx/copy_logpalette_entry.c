/* copy_logpalette_entry: candidate for 1068:0630 (34 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `copy_logpalette(src, dst)`, from an entry that takes one more argument FIRST and never
 * reads it -- the shape of a member function that ignores its `this`. Nothing calls it, so
 * what that leading argument is is not shown.
 *
 * copy_logpalette is in the widget segment, a call, so it is extern.
 */

#include "colonization.h"

void copy_logpalette_entry(void *unused, void *src, void *dst)
{
    copy_logpalette(src, dst);
}
