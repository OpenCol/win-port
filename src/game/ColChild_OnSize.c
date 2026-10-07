/* ColChild_OnSize: candidate for 1048:0a3b (333 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_OnSize
 * name-source: sibling-build
 *
 * The other end of `ColChild_OnMove`, 0x14d bytes above it: it re-lays the
 * current window's client rectangle at +0x76a -- inset 0xa from the left and 0x20 from the
 * top, ten short of the window's own width and height -- notifies through handler slot 0x1894,
 * and then records the placement with THE SAME twenty-line tail the closer uses: iconic is 1
 * and nothing more; zoomed is 2 and otherwise 0, both with the rectangle copied out field by field.
 *
 * The placement record is looked up FIRST, before any of the layout happens, and only used at
 * the end. A window with no record still gets laid out and still notifies.
 *
 * The `- 0x26` walk back from `GetEventWindow` is the same conditional the closer does, and for
 * the same reason: the null case must not be adjusted.
 *
 * `ColChild_DrawTitleBar` comes through the five-byte NOP form and is extern; every other callee
 * has the four-byte form and is a stub here (rule 47).
 *
 * RENAMED 2026-09-26 from window_resized: the Mac build's ColChild::OnSize(short): the size handler, taking the size code and re-laying the current window's client rectangle. The width matches.
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
extern PlacementRec *GamePrefs_GetWindow(void *root, int which);

extern void MSSetFocus_of_1048_e0cd(Screen *w);
extern void DialogWin_SetDefaultControlFocus(DialogWin *w);
extern int MSIsWindowMinimized(void *win);
extern int MSIsWindowMaximized(void *win);
extern void MSGetSizeParent(MSWin *win, RECT *r);

/* 5-byte NOP form, so extern (rule 47) */
extern void ColChild_DrawTitleBar(ColChild *o);

extern "C" int MSIsWindowMinimized_of_1048_d8ad(Screen *w);

extern "C" int MSIsWindowMaximized_of(Screen *w);

extern "C" void MSGetSizeParent_of(Screen *w, RECT *r);

extern "C" int port_width_1048_d853(GRPort *port);
extern "C" int port_height_1048_d86c(GRPort *port);
extern "C" void call_handler_slot_1894(ColChild *w, int size_code);

void ColChild_OnSize(int size_code)
{
    ColChild *o;
    PlacementRec *p;
    RECT rc;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (ColChild *)w;
    p = GamePrefs_GetWindow(&g_prefs_root, o->which);
    SetRect(&o->rect, 0xa, 0x20, port_width_1048_d853(o) - 10, port_height_1048_d86c(o) - 10);
    ColChild_DrawTitleBar(o);
    call_handler_slot_1894(o, size_code);
    MSDoAllTasks_1048_d701();
    if (p != 0 && o->save_flag != 0) {
        if (MSIsWindowMinimized_of_1048_d8ad(&(DialogWin &)*o))
            p->state = 1;
        else {
            if (MSIsWindowMaximized_of(&(DialogWin &)*o))
                p->state = 2;
            else
                p->state = 0;
            MSGetSizeParent_of(&(DialogWin &)*o, &rc);
            p->rect.left = rc.left;
            p->rect.top = rc.top;
            p->rect.right = rc.right;
            p->rect.bottom = rc.bottom;
        }
    }
}
