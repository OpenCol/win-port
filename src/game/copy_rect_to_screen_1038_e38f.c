/* PORT: this file's own declarations of port_validate_rect_1038_e582 removed: the header declares it from the definition (W1 step 3). */
/* copy_rect_to_screen_1038_e38f: candidate for 1038:e38f (153 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: copy_rect_to_screen_1038_e38f
 *
 * The copy in segment 1038 of copy_rect_to_screen (1048:d963), calling its own module's
 * validate helper. Copies a window's off-screen port to the screen and tells a per-module helper what was
 * copied: the rectangle given, or the window's own bounds when none is. The same 153
 * bytes occur in fifteen segments, each calling its own module's helper -- one method of
 * a base class compiled into every module that uses it.
 */

#include "colonization.h"

#pragma option -a
#include "w16types.h"
#include "classes.h"
#pragma option -a-
class DlgControl;
extern void CopyToScreen(GRPort *port, Screen *screen, RECT *src, RECT *dst);

void copy_rect_to_screen_1038_e38f(GameWin *w, RECT *r)
{
    if (r != 0) {
        CopyToScreen(w, &(DialogWin &)*w, r, r);
        port_validate_rect_1038_e582(&(DialogWin &)*w, r);
    } else {
        CopyToScreen(w, &(DialogWin &)*w, &w->bounds, &w->bounds);
        port_validate_rect_1038_e582(&(DialogWin &)*w, &w->bounds);
    }
}
