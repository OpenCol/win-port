/* track_popup_submenu: candidate for 1048:e1fd (59 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Pops up a submenu of `m` over the window that `w` owns, at client point (x, y):
 * `MSTrackPopupMenu` is the layer that maps the point through `ClientToScreen` and calls
 * `TrackPopupMenu` on the HWND at +2 of the window.
 *
 * The two helpers are this module's own, both reached by the four-byte `PUSH CS /
 * CALL NEAR` form, so both are in this translation unit and appear here as stubs (rule 47);
 * `MSTrackPopupMenu` is a call and is `extern`. Their real bodies are filed separately:
 * `get_win_1048_e6d0` is the pointer at +6, `submenu_of_1048_e8c1` is
 * `MSGetSubMenu` on the handle at +0.
 *
 * Argument evaluation is right to left, which is why the window is fetched before the
 * menu even though the menu is the first argument.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSTrackPopupMenu(unsigned hmenu, MSWin *win, int x, int y);

/* same object: 4-byte `push cs; call ` */
extern void *get_win_1048_e6d0(void *o);
extern unsigned submenu_of_1048_e8c1(void *menu, int pos);

void track_popup_submenu(Screen *w, MenuClass *menu, int x, int y)
{
    MSTrackPopupMenu(submenu_of_1048_e8c1(menu, 0), (MSWin *)get_win_1048_e6d0(w), x, y);
}
