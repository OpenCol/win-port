/* MapWin_RightMouseClick: candidate for 1038:950c (305 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_RightMouseClick
 *
 * A right click on the map window (re-entry flag at DGROUP:0x1c52): outside the map
 * rectangle (+0x76a..+0x770) the window is handled by win_show_and_focus_1038_e279; inside, the click is
 * converted to a map position and, in view mode 0 with the selected unit standing there,
 * track_popup_submenu_1038_e2b5 opens the unit's menu (kind 2). MSDoAllTasks_1038_e2a2 runs afterwards either way.
 */

#include "colonization.h"

#include "w16types.h"
class DlgControl { public: short kind; short id; void *win; RECT rect; short hwnd; DlgControl *next; char initial_focus; char hotkey; short hotkey_pos; DlgControl(); ~DlgControl(); };
class SButton : public DlgControl { public: short enabled; long action; short colour; SButton(); ~SButton(); };
#pragma option -a
class Screen { public: short style; void *ctable; void *win; short fa; char disabled; Screen(); ~Screen(); };
#pragma option -a-
class EventTable { public: void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; EventTable(); ~EventTable(); };
class EventWin : public Screen, public EventTable { public: char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; EventWin(); ~EventWin(); };
class DialogWin : public EventWin { public: DlgControl *first_ctl; DlgControl *ok_btn; DlgControl *cancel_btn; char keyboard_nav; char f99; DialogWin(); ~DialogWin(); };
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
class GameWin : public GRPort, public DialogWin { public: void *relayout; GameWin(); ~GameWin(); virtual int ResizePort(RECT *r); };
class SImageButton : public DlgControl { public: long on_click; long on_rclick; GRPort up; GRPort down; void *ctable; char reinit_faces; char pad_73; SImageButton(); ~SImageButton(); };
class ColChild : public GameWin { public: char title[0x80]; int tile_w, tile_h; char save_flag; char own_msg[0xa0]; char queue[0x10][0x50]; char pad_6e9; SImageButton btn; unsigned char active; unsigned char kind; int dropped; void *repaint; void *resize; RECT rect; int which; char noclose; char pad_775; int msg_timer; int queued; ColChild(); ~ColChild(); };
class MapWin : public ColChild { public: short cx; short cy; char pad_77e[4]; short zoom; short drag; short lbutton_held; MapWin(); ~MapWin(); };
#include "records.h"

extern void *GetEventWindow(void);

extern GameState g_game;   /* SEG20:0x7782 */
typedef struct { short hmenu; } MenuClass;

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void win_show_and_focus_1038_e279(void *w);
extern int MapWin_WinPos2MapPos(MapWin *o, int x, int y, int *mx, int *my);
extern "C" void track_popup_submenu_1038_e2b5(void *w, void *menu, int kind, int x, int y);

void MapWin_RightMouseClick(int x, int y)
{
    MapWin *o;
    int mx;
    int my;
    EventWin *w;

    if (g_map_click_busy != 0)
        return;
    g_map_click_busy = 1;
    w = (EventWin *)GetEventWindow();
    o = (MapWin *)w;
    if (o->rect.left > x || o->rect.right < x || o->rect.top > y || o->rect.bottom < y) {
        win_show_and_focus_1038_e279(&(DialogWin &)*o);
    } else {
        x -= o->rect.left;
        y -= o->rect.top;
        MapWin_WinPos2MapPos(o, x, y, &mx, &my);
        if (g_game.view_mode == 0 && g_units[g_game.selected_unit].x == mx && g_units[g_game.selected_unit].y == my)
            track_popup_submenu_1038_e2b5(&(DialogWin &)*o, &g_menu_owner, 2, x, y);
    }
    MSDoAllTasks_1038_e2a2();
    g_map_click_busy = 0;
}
