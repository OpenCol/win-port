/* MSTrackPopupMenu: candidate for 1070:d996 (77 bytes).  Verdict: RELOCATABLE.
 *
 * Pops a menu up at a point given in the window's CLIENT coordinates: the point is copied
 * into a local, converted with `ClientToScreen`, and handed to `TrackPopupMenu` with
 * `TPM_RIGHTBUTTON` (2; TPM_CENTERALIGN would be 4), no reserved value, the window as owner and no bounding rectangle.
 * That conversion is the whole reason the wrapper exists -- `TrackPopupMenu` takes screen
 * coordinates and every caller here has client ones.
 *
 * The name follows the family rather than being invented for it: this layer spells its
 * menu wrappers `MS` plus the Win16 call they wrap, and `MSDestroyMenu`, `MSGetSubMenu`
 * and `MSInitMenu` are already in the corpus. `MSGetSubMenu` is the next function in the
 * segment.
 *
 * Unlike its neighbours this one does NOT guard against a null menu handle, which
 * `MSDestroyMenu` and `MSGetSubMenu` both do. `TrackPopupMenu` fails harmlessly on one, so
 * it is not a bug, but it is a real asymmetry in a family that otherwise checks.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

void MSTrackPopupMenu(unsigned hmenu, MSWin *win, int x, int y)
{
    POINT pt;

    pt.x = x;
    pt.y = y;
    ClientToScreen(win->hwnd, &pt);
    TrackPopupMenu(hmenu, 2, pt.x, pt.y, 0, win->hwnd, 0);
}
