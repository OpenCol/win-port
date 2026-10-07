/* port_copy_both: candidate for 1068:a9b3 (148 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_copy_both
 * name-source: descriptive
 *
 * Blits one rectangle into BOTH the port at +0xc4 and the object itself: the destination
 * rectangle goes to the port's virtual first slot, the port is selected against the pointer
 * at +2 of the sub-object at +0x26, and then the same four arguments are handed to
 * `GRPort_CopyToPort` twice with different destinations.
 *
 * Two blits from one source is the whole function, and it is why the argument list is
 * repeated instruction for instruction: `-Od` keeps nothing across a call, so all four
 * pointers are pushed again from the frame.
 *
 * The virtual call is why this file is C++ -- a vtable pointer reading a entry,
 * which only `virtual` produces.
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
class MagicWin : public GameWin { public: GRPort port; void *pal; RECT rect; int first, count; char bound; char fb; void *anim; MagicWin(); ~MagicWin(); };
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void *get_ctable_1068_d762(void *r);
typedef struct { char pal[0x404]; unsigned hpal; short seed; short steps, first, count; short steps2, first2, count2; unsigned char r, g, b; char f417; short hsrc_a, hsrc_b, hsrc; } CTable;   /* 0x41e bytes: the palette, its handle and the cycling state */
extern "C" void GRPort_ColorSync(GRPort *port, CTable *page);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

extern "C" void port_copy_both(MagicWin *o, void *src, RECT *sr,
                                             RECT *dr)
{
    o->port.ResizePort(dr);
    GRPort_ColorSync(&o->port, (CTable *)get_ctable_1068_d762((Screen *)&(DialogWin &)*o));
    GRPort_CopyToPort((GRPort *)src, &o->port, sr, dr);
    GRPort_CopyToPort((GRPort *)src, (GRPort *)o, sr, dr);
}
