/* PediaWin_Done: candidate for 1060:2a47 (176 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: PediaWin_Done
 * name-source: sibling-build
 *
 * Closes one of this module's windows, and the flag byte at +0x857 decides which of two
 * closes it gets. Clear -- an ordinary window -- and it is hidden, its port is blitted over
 * a 1x1 rectangle at the origin, and the object at +0x818 is given the same rectangle
 * through its VIRTUAL function. Set -- a modal one -- and none of that happens; instead
 * +0x858 is cleared and the modal session ended through `EventWin_EndModal`. Either way +0x77a goes to -1, and a non-modal
 * close hands the focus back to the game window at SEG20:0x5ca.
 *
 * `PediaWin_KeyClick` calls this on the Colonizopedia's owner and
 * `MainWin_ResetWindows` on the record at +0x32b2, so it is the module's generic close
 * rather than one screen's.
 *
 * THE CALL THROUGH +0x818 IS VIRTUAL, and that is what makes the member a class rather than
 * a pointer: `MOV BX,[ES:BX+0x818] / CALL FAR [BX]` reads a NEAR vptr out of the object and
 * calls through the entry it points at, which is Borland's C++ layout -- vtables in
 * DGROUP, entries. `this` is pushed after the argument, so it is the leftmost parameter.
 *
 * RENAMED 2026-09-26 from win_close_1060_2a47: the Mac build's PediaWin::Close(void): this only; it closes the Colonopedia (hidden, or its modal session ended when it was run modally), and its callers are the pedia object's -- MainWin_ResetWindows on o->pedia, MainWin_DefaultDesktop on g_pedia_win, and PediaWin_KeyClick. The width matches.
 *
 * RENAMED 2026-09-27 from PediaWin_Close: BEYOND THE PROPOSALS. It vacates PediaWin_Close for 1060:2bc3. Mac PediaWin::Done(void), CODE 20 0x153e, 152 bytes. Its seq is MSHideWindow, GameWin::ResizePort, EventWin::EndModal, MSSetFocus. Its callers are DefaultDesktop, OnOKClick, ResetWindows and (bsr) KeyClick. 2a47 (PediaWin *w), 176 bytes, calls MSHideWindow, GameWin_ResizePort, EventWin_EndModal and win_set_focus. Its callers are MainWin_DefaultDesktop, PediaWin_OnOKClick, MainWin_ResetWindows and PediaWin_KeyClick, four for four. Mac PediaWin::Close is a 50-byte GetEventWindow/return-true routine, which is 2bc3. Layout: Mac FillListArea, Done, CurrentEntry, OnOKClick, Close; Windows 282e, 2a47, CurrentEntry, OnOKClick, 2bc3. Apply it first in its chain. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
class Screen { public: short style; void *ctable; void *win; short fa; char disabled; Screen(); ~Screen(); };
#pragma option -a-
class EventTable { public: void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; EventTable(); ~EventTable(); };
class EventWin : public Screen, public EventTable { public: char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; EventWin(); ~EventWin(); };
class DlgControl; class DialogWin : public EventWin { public: DlgControl *first_ctl; DlgControl *ok_btn; DlgControl *cancel_btn; char keyboard_nav; char f99; DialogWin(); ~DialogWin(); };
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
class GameWin : public GRPort, public DialogWin { public: void *relayout; GameWin(); ~GameWin(); virtual int ResizePort(RECT *r); };
class DlgControl { public: short kind; short id; void *win; RECT rect; short hwnd; DlgControl *next; char initial_focus; char hotkey; short hotkey_pos; DlgControl(); ~DlgControl(); };
class SImageButton : public DlgControl { public: long on_click; long on_rclick; GRPort up; GRPort down; void *ctable; char reinit_faces; char pad_73; SImageButton(); ~SImageButton(); };
class ColChild : public GameWin { public: char title[0x80]; int tile_w, tile_h; char save_flag; char own_msg[0xa0]; char queue[0x10][0x50]; char pad_6e9; SImageButton btn; unsigned char active; unsigned char kind; int dropped; void *repaint; void *resize; RECT rect; int which; char noclose; char pad_775; int msg_timer; int queued; ColChild(); ~ColChild(); };
class SScrollBar : public DlgControl { public: long on_change; long on_track; short page; short f24; short pos; SScrollBar(); ~SScrollBar(); };
class VListItem { public: void *value; VListItem *next; VListItem *prev; VListItem(); ~VListItem(); };
class VList { public: VListItem *first; VListItem *last; VListItem *cur; VList(); ~VList(); };
#include "records.h"
template <class T> class LinkedList : public VList {
public:
    LinkedList();
};
class PediaWin : public ColChild { public: short category; SImageButton button; SScrollBar bar; GRPort port; LinkedList<PediaEntry> list; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; PediaWin(); ~PediaWin(); };

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;         /* SEG20:0x5ca */

extern void GameWin_ResizePort(GameWin *w, void *rect);
extern void EventWin_EndModal(void *port);

/* same object: 4-byte `push cs; call ` */
extern "C" void MSHideWindow_of_1060_691b(DialogWin *port);
extern "C" void win_set_focus(DialogWin *w);

void PediaWin_Done(PediaWin *w)
{
    RECT r;

    if (w->modal == 0) {
        MSHideWindow_of_1060_691b(&(DialogWin &)*w);
        SetRect(&r, 0, 0, 1, 1);
        GameWin_ResizePort(w, &r);
        w->port.ResizePort(&r);
    }
    w->category = -1;
    if (w->modal != 0) {
        w->initialised = 0;
        EventWin_EndModal(&(DialogWin &)*w);
    } else {
        win_set_focus((DialogWin *)&g_main_win);
    }
}

