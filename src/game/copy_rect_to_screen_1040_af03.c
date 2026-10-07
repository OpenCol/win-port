/* copy_rect_to_screen_1040_af03: candidate for 1040:af03 (153 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: copy_rect_to_screen_1040_af03
 *
 * The copy in segment 1040 of copy_rect_to_screen (1048:d963), calling its own module's
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
extern "C" void port_validate_rect_1040_b27d(DialogWin *port, RECT *rect);

void copy_rect_to_screen_1040_af03(GameWin *w, RECT *r)
{
    if (r != 0) {
        CopyToScreen(w, &(DialogWin &)*w, r, r);
        port_validate_rect_1040_b27d(&(DialogWin &)*w, r);
    } else {
        CopyToScreen(w, &(DialogWin &)*w, &w->bounds, &w->bounds);
        port_validate_rect_1040_b27d(&(DialogWin &)*w, &w->bounds);
    }
}
