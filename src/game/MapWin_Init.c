/* PORT: this file's own declarations of ColChild_InitCWin_style, MSSetCursor_of, set_command_handler_1038_e1ba, set_keyboard_nav_1038_e047, set_max_track_size_1038_e1f7, set_min_track_size_1038_e1d5, set_on_keydown_1038_e240, set_on_lbutton_down_1038_e060, set_on_lbutton_up_1038_e099, set_on_mousemove_1038_e126, set_on_rbutton_down_1038_e0d2, set_on_restore, set_repaint_handler_1038_e10b, win_set_icon_1038_e196 removed: the header declares them from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 43, 44, 45, 46, 47, 48, 50, 54 of the imported file. */
/* MapWin_Init: candidate for 1038:91f3 (382 bytes, 1 caller).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_Init
 *
 * Builds the map window: five fields (cx = cy = 1 and zoom 0 first, drag -1 and lbutton_held 0 last), the port at +0x26 configured through eight small
 * setters (creation as "Area Map" 300x300 style 0x1000, size 84x106, limits 928x1216),
 * and six event-slot handlers installed on the handler block at +0x33.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void MapWin_RedrawMap(void);
extern void MapWin_TrackMouse(int x, int y);

extern void MainWin_MainMenuProc(long cmd);
extern void KeyEvents(int key);

void MapWin_Init(MapWin *w)
{
    w->cx = w->cy = 1;
    w->zoom = 0;
    set_keyboard_nav_1038_e047(&(DialogWin &)*w, 0);
    ColChild_InitCWin_style(w, g_title_area_map, 2, 0, 0, 300, 300, 0x1000, 1);
    set_on_lbutton_down_1038_e060(&(EventTable &)*w, (void*)(MapWin_LeftMouseDown));
    set_on_lbutton_up_1038_e099(&(EventTable &)*w, (void*)(MapWin_LeftMouseUp));
    set_on_rbutton_down_1038_e0d2(&(EventTable &)*w, (void*)(MapWin_RightMouseClick));
    set_repaint_handler_1038_e10b(w, (void*)(MapWin_RedrawMap));
    set_on_mousemove_1038_e126(&(EventTable &)*w, (void*)(MapWin_TrackMouse));
    set_on_restore(&(EventTable &)*w, (void*)(recenter_map_on_cursor));
    win_set_icon_1038_e196(&(DialogWin &)*w, 0x80);
    set_command_handler_1038_e1ba(&(DialogWin &)*w, (void*)(MainWin_MainMenuProc));
    set_min_track_size_1038_e1d5(&(DialogWin &)*w, 0x54, 0x6a);
    set_max_track_size_1038_e1f7(&(DialogWin &)*w, 0x3a0, 0x4c0);
    MSSetCursor_of(&(DialogWin &)*w, -1, 0);
    set_on_keydown_1038_e240(&(EventTable &)*w, (void*)(KeyEvents));
    w->drag = -1;
    w->lbutton_held = 0;
}
