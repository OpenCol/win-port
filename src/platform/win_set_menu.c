/* win_set_menu: candidate for 1078:27d5 (93 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -r-
 * symbol: win_set_menu
 *
 * Attaches a menu to the window and grows the window by one menu bar so the client area is unchanged.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

extern int nonclient_width(MSWin *win);
extern int nonclient_height(MSWin *win);

extern int win_client_width_1078_23db(MSWin *win);

extern int win_client_height_1078_241a(MSWin *win);

extern void win_set_client_size_1078_2059(MSWin *win, int cw, int ch);

void win_set_menu(MSWin *win, unsigned hmenu)
{
    if (win == 0)
        return;
    SetMenu(win->hwnd, hmenu);
    win_set_client_size_1078_2059(win, win_client_width_1078_23db(win),
                                  GetSystemMetrics(15) + win_client_height_1078_241a(win) + 1);
}
