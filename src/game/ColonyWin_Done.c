/* ColonyWin_Done: candidate for 1010:b004 (54 bytes, 1 caller).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_Done
 * name-source: sibling-build
 *
 * Hides the window (its DialogWin at +0x26), sets its colony (+0xa12) to -1 and gives the
 * focus back to the main window.
 *
 * RENAMED 2026-09-26 from win_close_and_reset: the Mac build's ColonyWin::Done(void): this only; it hides the colony window, sets its colony to -1 and gives the focus back to the main window -- what EuropeWin_Done does for Europe -- and ColonyWin_Close calls it. The width matches.
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
class SRegion : public DlgControl { public: long on_enter; long on_leave; long on_click; long on_press; long on_dblclk; SRegion(); ~SRegion(); };
class ColonyWin : public ColChild { public: SRegion region; GRPort back; GRPort terrain; SImageButton ib[3]; SButton b[2]; SImageButton ib2; char buy_shown; char change_shown; short panel; short cx; short cy; int timer; int colony; ColonyWin(); ~ColonyWin(); };
typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;   /* SEG20:0x05ca */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void MSHideWindow_of_1010_eb6e(void *port);
extern "C" void MSSetFocus_of_1010_eb8f(void *p);
void ColonyWin_Done(ColonyWin *w)
{
    MSHideWindow_of_1010_eb6e(&(DialogWin &)*w);
    w->colony = -1;
    MSSetFocus_of_1010_eb8f((Screen *)&g_main_win);
}
