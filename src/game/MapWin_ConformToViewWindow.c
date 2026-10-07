/* MapWin_ConformToViewWindow: candidate for 1038:a05f (147 bytes, 2 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_ConformToViewWindow
 * name-source: sibling-build
 *
 * If the rectangle's origin plus its extent overshoots the window's extent (the rect at
 * +0x76a), pull its edge back to the window's. Two extent helpers are called on both.
 *
 * RENAMED 2026-09-27 from clamp_rect_to_window: 8 bytes = (Rect*)+this; callers MapWin_ShowMap, MapWin_UpdateTerrain = Mac callers; calls only rect helpers (Mac: nothing)
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */

void MapWin_ConformToViewWindow(MapWin *w, RECT *r)
{
    int win_w, win_h, x, y, r_w, r_h;

    win_w = rect_width_1038_dfbd(&w->rect);
    win_h = rect_height_1038_dfdc(&w->rect);
    r_w = rect_width_1038_dfbd(r);
    r_h = rect_height_1038_dfdc(r);
    x = r->left;
    y = r->top;
    if (x + r_w > win_w)
        r->right = win_w;
    if (y + r_h > win_h)
        r->bottom = win_h;
}
