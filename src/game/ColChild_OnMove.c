/* ColChild_OnMove: candidate for 1048:0b88 (263 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_OnMove
 * name-source: sibling-build
 *
 * When a game window moves: the window is given the focus and its placement record (preferences
 * root SEG20:0x10, slot at +0x772) updated when the window's flag at +0x148 is set -- code 1
 * when minimised, otherwise 2 when maximised or 0 with the window rectangle recorded. The
 * module's window wrappers are defined here because the shipped calls to them are the
 * compiler's own `push cs / call ` (win_show_and_focus_1048_d8f7's own callee is not in this file).
 *
 * RENAMED 2026-09-26 from save_window_placement: the Mac build's ColChild::OnMove(short, short): game_window_init installs it on the EventTable's move slot, beside ColChild_OnSize on the size slot, and it records the window's placement in the preferences. The body reads no argument, so the Windows prototype has none.
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

extern "C" void win_show_and_focus_1048_d8f7(DialogWin *w);

extern "C" int MSIsWindowMinimized_of_1048_d8ad(Screen *w);

extern "C" int MSIsWindowMaximized_of(Screen *w);

extern "C" void MSGetSizeParent_of(Screen *w, RECT *r);

void ColChild_OnMove(void)
{
    ColChild *o;
    PlacementRec *p;
    RECT rc;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (ColChild *)w;
    win_show_and_focus_1048_d8f7(&(DialogWin &)*o);
    p = GamePrefs_GetWindow(&g_prefs_root, o->which);
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
