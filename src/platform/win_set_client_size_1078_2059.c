/* win_set_client_size_1078_2059: candidate for 1078:2059 (175 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -r-
 * symbol: win_set_client_size_1078_2059
 *
 * Resizes the window so its client area is the given size, keeping the top-left corner where it is in the parent's coordinates.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

extern int nonclient_width(MSWin *win);
extern int nonclient_height(MSWin *win);

extern int win_client_width_1078_23db(MSWin *win);

extern int win_client_height_1078_241a(MSWin *win);

void win_set_client_size_1078_2059(MSWin *win, int cw, int ch)
{
    RECT r;
    int fw;
    int fh;
    unsigned parent;
    POINT p;

    if (win == 0)
        return;
    GetWindowRect(win->hwnd, &r);
    fw = nonclient_width(win) + cw;
    fh = nonclient_height(win) + ch;
    if ((parent = GetParent(win->hwnd)) != 0) {
        p.x = 0;
        p.y = 0;
        MapWindowPoints(parent, 0, &p, 1);
        r.left -= p.x;
        r.top -= p.y;
    }
    MoveWindow(win->hwnd, r.left, r.top, fw, fh, 1);
}

extern void win_set_menu(MSWin *win, unsigned hmenu);
