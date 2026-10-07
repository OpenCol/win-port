/* PORT: `(int *)&g_cursors[10]` takes the address: the corpus's char g_cursors[][4] row decayed to its address; the header's element is a CursorClass (W1). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 53, 55 of the imported file. */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 53, 58 of the imported file. */
/* PORT: this file's own declarations of ColChild_InitCWin_style, MiniWin_LeftMouseClick, get_window_rect_1060_61dd, set_keyboard_nav_1060_615c, set_max_track_size_1060_6197, set_min_track_size_1060_6175, set_on_keydown_1060_629f, set_on_rbutton_down_1060_623c, set_repaint_handler_1060_62d8, win_set_cursor_1060_6275 removed: the header declares them from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 61, 66 of the imported file. */
/* MiniWin_Init: candidate for 1060:05e8 (313 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The overview map's (MiniWin's) setup, the same shape as `MapWin_Init`
 * (1038:91f3) with different numbers: created as "Map" at 200x200 style 0x1000 rather than
 * "Area Map" at 300x300, icon 0x83 rather than 0x80, and only three event slots installed
 * instead of six.
 *
 * FOUR GLOBALS ARE SET BEFORE THE WINDOW EXISTS: the view's tile size at DGROUP:0x4e0c/0x4e0e
 * to 2 and its origin at 0x4e10/0x4e12 to 0, so this window draws the map at two pixels to the
 * square from the top-left corner. `MiniWin_UpdateTerrain` multiplies by the first pair and
 * adds the second.
 *
 * One of the setter arguments is not a constant: the minimum tracking height (the y argument of `set_min_track_size_1060_6175`) comes from the
 * window's own word at +0x76c plus 0x55, read AFTER `ColChild_InitCWin_style` has created the window and
 * filled it in.
 *
 * The rectangle at +0x77e is filled by `get_window_rect_1060_61dd` after the icon is set and before the handlers go in, so the object
 * carries its own bounds.
 *
 * The port is `w->port`, a member and therefore a TYPED pointer: the shipped computes the
 * offset before pushing the segment, which a `void *` parameter would not do (rule 56).
 *
 * RENAMED 2026-09-26 from map_window_init: the Mac build's MiniWin::Init(void). This creates the 200x200 "Map" window (MapWin_Init makes the 300x300 "Area Map"), and the object it fills is 0x786 bytes -- MiniWin's size per MiniWin_ctor, with the rectangle at +0x77e ending exactly there.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void KeyEvents(int key);
extern void MiniWin_MiniUpdate(void);

/* same object: 4-byte `push cs; call ` */
extern void win_set_icon_1060_61b9(char *port, int icon);
extern void *set_on_lbutton_down_1060_6203(char *h, void *fn);

void MiniWin_Init(MiniWin *w)
{
    set_keyboard_nav_1060_615c((DialogWin *)((char *)w + 0x26), 0);
    w->f77a = 0;
    g_view_tile_w = 2;
    g_view_tile_h = 2;
    g_view_org_x = 0;
    g_view_org_y = 0;
    ColChild_InitCWin_style((ColChild *)w, "Map", 3, 0xc8, 0x64, 0xc8, 0xc8, 0x1000, 1);
    set_min_track_size_1060_6175((EventWin *)((char *)w + 0x26), 0x50, w->rect.top + 0x55);
    set_max_track_size_1060_6197((EventWin *)((char *)w + 0x26), 0x180, 0x1a0);
    win_set_icon_1060_61b9((char *)((DialogWin *)((char *)w + 0x26)), 0x83);
    get_window_rect_1060_61dd((ColChild *)((MiniWin *)((ColChild *)w)), &w->clip);
    set_on_lbutton_down_1060_6203((char *)((EventTable *)((char *)w + 0x33)), (void *)MiniWin_LeftMouseClick);
    set_on_rbutton_down_1060_623c((EventTable *)((char *)w + 0x33), (void *)MiniWin_RightMouseClick);
    win_set_cursor_1060_6275((DialogWin *)((char *)w + 0x26), (int *)&g_cursors[10], 0);
    set_on_keydown_1060_629f((EventTable *)((char *)w + 0x33), (void *)KeyEvents);
    set_repaint_handler_1060_62d8((ColChild *)((MiniWin *)((ColChild *)w)), (void *)MiniWin_MiniUpdate);
}
