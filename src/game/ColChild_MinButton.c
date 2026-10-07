/* ColChild_MinButton: candidate for 1048:0c8f (133 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_MinButton
 * name-source: sibling-build
 *
 * On minimizing a game window: its placement record (preferences root SEG20:0x10, slot at
 * +0x772) gets code 1 when the window's flag at +0x148 is set, and the window is minimized
 * through the module's wrapper (`win_minimize_of` -> `win_minimize`) (defined here because the shipped call is the compiler's own
 * `push cs / call `).
 *
 * RENAMED 2026-09-26 from mark_placement: the Mac build's ColChild::MinButton(short): the name audit found this is the action behind the title-bar minimize button (it records code 1, minimized, in the window's placement record and minimizes the window), and LAYOUT had already put MinButton here -- between ColChild_OnMove and ColChild_DrawTitleBar on both sides -- held only for arity; a handler that reads no argument takes none, the Windows handlers' usual shape.
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
extern void win_minimize(void *win);

extern "C" void win_minimize_of(DialogWin *w);

void ColChild_MinButton(void)
{
    ColChild *o;
    PlacementRec *p;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (ColChild *)w;
    p = GamePrefs_GetWindow(&g_prefs_root, o->which);
    if (p != 0 && o->save_flag != 0)
        p->state = 1;
    win_minimize_of(&(DialogWin &)*o);
}
