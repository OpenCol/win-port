/* MiniWin_LeftMouseClick: candidate for 1060:076c (205 bytes, ? callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A click at (x, y): outside the current window's rectangle (+0x77e..+0x784, the object
 * 0x26 bytes below the window pointer) it is handed to win_take_focus with the window;
 * inside, the offset from the rectangle's origin plus the view origin is divided by the
 * tile size and the map is recentred on that tile.
 *
 * RENAMED 2026-09-26 from map_click_to_tile: the Mac build's MiniWin::LeftMouseClick(short, short): MiniWin_Init installs it as the window's lbutton_down handler, and it takes the same two shorts.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);
/* same object: 4-byte `push cs; call ` (rule 47) */

void MiniWin_LeftMouseClick(int x, int y)
{
    MiniWin *c;
    int tx, ty;
    char *w = (char *)GetEventWindow();

    c = (MiniWin *)(w != 0 ? w - 0x26 : w);
    if (c->clip.left > x || c->clip.right < x || c->clip.top > y || c->clip.bottom < y) {
        win_take_focus((DialogWin *)((char *)c + 0x26));
        return;
    }
    x -= c->clip.left + g_view_org_x;
    y -= c->clip.top + g_view_org_y;
    tx = x / g_view_tile_w;
    ty = y / g_view_tile_h;
    MapWin_Recenter(&g_map_window, tx, ty);
}
