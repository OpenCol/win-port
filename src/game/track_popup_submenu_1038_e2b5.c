/* PORT: this file's own declarations of get_win_1038_e372, submenu_of_1038_e55f removed: the header declares them from the definition (W1 step 3). */
/* track_popup_submenu_1038_e2b5: candidate for 1038:e2b5 (60 bytes).  Verdict: RELOCATABLE.
 * symbol: track_popup_submenu_1038_e2b5
 *
 * MSTrackPopupMenu with the submenu handle from submenu_of_1038_e55f and the window from
 * get_win_1038_e372.  Both helpers are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from menu_add_item: Calls MSTrackPopupMenu with a submenu handle (submenu_of_1038_e55f = MSGetSubMenu) over the window from get_win_1038_e372 at (x, y): it pops up and tracks a menu, adding nothing. It is the byte-level 1038 twin of track_popup_submenu (1048:e1fd); its only caller, MapWin_RightMouseClick, uses it to open the unit's right-click menu. 'menu_add_item' claims a menu-construction step the body never performs.
 */

#include "colonization.h"

#include "classes.h"
extern int g_stub_a;
#include "records.h"
extern void MSTrackPopupMenu(unsigned hmenu, MSWin *win, int x, int y);


void track_popup_submenu_1038_e2b5(void *w, void *menu, int submenu_pos, int x, int y)
{
    MSTrackPopupMenu(submenu_of_1038_e55f((MenuClass *)menu, submenu_pos), (MSWin *)(get_win_1038_e372((Screen *)w)), x, y);
}
