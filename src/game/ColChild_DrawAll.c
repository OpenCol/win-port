/* ColChild_DrawAll: candidate for 1048:0d14 (443 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_DrawAll
 * name-source: sibling-build
 *
 * The geometry handler for one of the game's windows. It relays out the client area -- the
 * window's own rectangle at +0x76a inset by 10 on the right and bottom and starting at (0xa,
 * 0x20), and the control at +0x6ea moved to hang 0x20 in from the right edge -- and then
 * RECORDS where the window is, in the preferences entry `GamePrefs_GetWindow` looks up by
 * the window's own key at +0x772.
 *
 * WHAT IS RECORDED DEPENDS ON THE STATE. Iconic writes code 1 and NOTHING ELSE -- the
 * rectangle is left as it was, so a window minimised and quit comes back where it last stood
 * rather than in the corner. Zoomed writes code 2 and the rectangle; normal writes 0 and the
 * rectangle. `MainWin_Init` reads the same three codes back.
 *
 * The byte at +0x148 is what says a window's placement is worth saving at all; a window with
 * it clear gets its layout redone and nothing written.
 *
 * The rectangle built into the local `r` for the control's position is passed to nothing --
 * `MSGetSizeParent_of` overwrites it before the only read. Two `SetRect`s, and the
 * second one's result is dead.
 *
 * RENAMED 2026-09-27 from save_win_placement: the Mac build's ColChild_DrawAll, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: current_window's ColChild, relayout, ColChild_DrawNonClient, SetEventWindow, repaint, GamePrefs_GetWindow: the Mac ColChild::DrawAll sequence (GetEventWindow, DrawNonClient, SetEventWindow, GamePrefs::GetWindow); a handler with no this; ratio 0.62.
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
#include "records.h"
typedef struct { long f0; short w; short h; short count; short use_default_layout; PlacementRec windows[32]; short slots[8]; } GamePrefs;

extern void *GetEventWindow(void);
extern void SetEventWindow(void *w);
extern PlacementRec *GamePrefs_GetWindow(void *root, int which);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int port_width_1048_d853(ColChild *port);
extern "C" int port_height_1048_d86c(ColChild *port);

extern "C" void ctl_move(void *control, int x, int y);
extern "C" void ColChild_DrawNonClient(ColChild *o);
extern "C" void call_repaint_handler(ColChild *table);
extern "C" int MSIsWindowMinimized_of_1048_d8ad(Screen *w);
extern "C" int MSIsWindowMaximized_of(Screen *w);
extern "C" void MSGetSizeParent_of(Screen *w, RECT *r);

void ColChild_DrawAll(void)
{
    ColChild *o;
    PlacementRec *e;
    RECT r;
    EventWin *w;

    w = (EventWin *)GetEventWindow();
    o = (ColChild *)w;
    SetRect(&o->rect, 0xa, 0x20,
            port_width_1048_d853(o) - 10, port_height_1048_d86c(o) - 10);
    SetRect(&r, port_width_1048_d4f6(o) - 0x21, 3, port_width_1048_d4f6(o) - 0xe, 0x1d);
    ctl_move(&o->btn, port_width_1048_d4f6(o) - 0x20, 0);
    ColChild_DrawNonClient(o);
    SetEventWindow((DialogWin *)o);
    call_repaint_handler(o);
    e = GamePrefs_GetWindow(&g_prefs_root, o->which);
    if (e != 0 && o->save_flag != 0) {
        if (MSIsWindowMinimized_of_1048_d8ad(&(DialogWin &)*o) != 0) {
            e->state = 1;
        } else {
            if (MSIsWindowMaximized_of(&(DialogWin &)*o) != 0)
                e->state = 2;
            else
                e->state = 0;
            MSGetSizeParent_of(&(DialogWin &)*o, &r);
            e->rect.left = r.left;
            e->rect.top = r.top;
            e->rect.right = r.right;
            e->rect.bottom = r.bottom;
        }
    }
}
