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
extern "C" void set_keyboard_nav_1038_e047(void *port, int keyboard_nav);
extern void ColChild_InitCWin_style(void *o, char *title, int which, int x, int y, int cx, int cy, int style, int save);
extern "C" void set_on_lbutton_down_1038_e060(void *h, void *fn);
extern "C" void set_on_lbutton_up_1038_e099(void *h, void *fn);
extern "C" void set_on_rbutton_down_1038_e0d2(void *h, void *fn);
extern "C" void set_repaint_handler_1038_e10b(MapWin *w, void *fn);
extern "C" void set_on_mousemove_1038_e126(void *h, void *fn);
extern "C" void set_on_restore(void *h, void *fn);
extern "C" void win_set_icon_1038_e196(void *port, int icon);
extern "C" void set_command_handler_1038_e1ba(void *port, void *fn);
extern "C" void set_min_track_size_1038_e1d5(void *port, int min_track_w, int min_track_h);
extern "C" void set_max_track_size_1038_e1f7(void *port, int max_track_w, int max_track_h);
extern "C" void MSSetCursor_of(void *port, int cur, int now);
extern "C" void set_on_keydown_1038_e240(void *h, void *fn);

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
    set_on_lbutton_down_1038_e060(&(EventTable &)*w, MapWin_LeftMouseDown);
    set_on_lbutton_up_1038_e099(&(EventTable &)*w, MapWin_LeftMouseUp);
    set_on_rbutton_down_1038_e0d2(&(EventTable &)*w, MapWin_RightMouseClick);
    set_repaint_handler_1038_e10b(w, MapWin_RedrawMap);
    set_on_mousemove_1038_e126(&(EventTable &)*w, MapWin_TrackMouse);
    set_on_restore(&(EventTable &)*w, recenter_map_on_cursor);
    win_set_icon_1038_e196(&(DialogWin &)*w, 0x80);
    set_command_handler_1038_e1ba(&(DialogWin &)*w, MainWin_MainMenuProc);
    set_min_track_size_1038_e1d5(&(DialogWin &)*w, 0x54, 0x6a);
    set_max_track_size_1038_e1f7(&(DialogWin &)*w, 0x3a0, 0x4c0);
    MSSetCursor_of(&(DialogWin &)*w, -1, 0);
    set_on_keydown_1038_e240(&(EventTable &)*w, KeyEvents);
    w->drag = -1;
    w->lbutton_held = 0;
}
