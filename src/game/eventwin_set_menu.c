/* eventwin_set_menu: candidate for 1060:ab8b (63 bytes).  Verdict: RELOCATABLE.
 * symbol: eventwin_set_menu
 *
 * Passes the pointer's window form (get_w_0) to win_set_menu on the
 * object's window (get_win_1060_ab6e) and stores the pointer at +0x6a.  Both helpers
 * are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from win_set_long: Calls win_set_menu (SetMenu plus growing the window by one menu bar) on the object's window with the menu's handle, then stores the menu object at EventWin+0x6a, the same field win_redraw_menu_bar tests as the window's menu. So its job is attaching a menu to the window. 'win_set_long' reads like a SetWindowLong wrapper that stores a long, and says nothing about the menu.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern char g_stub_s[];
extern int g_stub_a;
extern void win_set_menu(void *win, int u);

extern int get_w_0(void *p);
extern void *get_win_1060_ab6e(void *o);

void eventwin_set_menu(EventWin *o, void *p)
{
    win_set_menu((MSWin *)(get_win_1060_ab6e(o)), get_w_0((MenuClass *)p));
    o->menu = p;
}
