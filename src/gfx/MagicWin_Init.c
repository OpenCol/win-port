/* MagicWin_Init: candidate for 1068:a890 (150 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MagicWin_Init
 * name-source: sibling-build
 *
 * Builds a control with a port: the rectangle is `SetRect(&r, 0, 0, w, h)`, the eleven words
 * the caller hands over go to `GameWin_Init` (1060:a16b -- the shipped call's relocation
 * names a16b, per tools/check_data_refs.py; an earlier reading had `init_and_set_1060_a11e`,
 * which the twin calls), the control is subclassed against the window at +0x26, and the port
 * at +0xc4 is told the rectangle through the first slot of its vtable.
 *
 * ITS TWIN AT 1068:a7fa IS THE SAME FOUR STEPS IN A DIFFERENT ORDER, with a different second
 * step. This one calls the virtual FIRST, before the control is built, and builds it with
 * `GameWin_Init`; that one calls it LAST, after the subclass, and builds with
 * `init_and_set_1060_a11e` (GameWin_Init plus `eventwin_set_menu`). Both are 150 bytes and
 * every instruction is shared apart from that callee; the order is what Borland's
 * source-order emission makes visible and nothing else would.
 *
 * The virtual call is why this file is C++: the vtable pointer is a NEAR word at the start of
 * the sub-object and the entry it reads is a function, and `-P` with one `virtual`
 * declaration produces both.
 *
 * `SetRect` is ``, so its arguments go left to right and the width and height are the
 * LAST pair pushed.
 *
 * RENAMED 2026-10-02 from build_control_1068_a890: the Mac build's MagicWin::Init(char*, short,
 * short, short, short, short, CTable&, Screen&) -- the same eight arguments after `this`, which
 * this function hands to GameWin_Init -- and its one caller is ClosingWin's constructor, a
 * MagicWin subclass, passing `this`. The `Obj` view became the MagicWin class the same day.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
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

typedef struct { char pal[0x404]; unsigned hpal; short seed; short steps, first, count; short steps2, first2, count2; unsigned char r, g, b; char f417; short hsrc_a, hsrc_b, hsrc; } CTable;   /* 0x41e bytes: the palette, its handle and the cycling state */
extern "C" void GameWin_Init(GameWin *o, char *name, int style, int x, int y,
                                 int w, int h, CTable *pal, Screen *parent);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void *get_win_1068_d72a(void *o);
extern "C" void subclass_control_1070_bb63(MagicWin *obj, void *hwnd);

extern "C" void MagicWin_Init(MagicWin *o, char *name, int style, int x, int y,
                             register int w, register int h, void *pal, void *parent)
{
    RECT r;

    SetRect(&r, 0, 0, w, h);
    o->port.ResizePort(&r);
    GameWin_Init((GameWin *)o, name, style, x, y, w, h, (CTable *)pal, (Screen *)parent);
    subclass_control_1070_bb63(o, (MSWin *)(get_win_1068_d72a((Screen *)(&(DialogWin &)*o))));
}
