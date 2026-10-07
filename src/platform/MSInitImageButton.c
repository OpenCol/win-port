/* MSInitImageButton: candidate for 1070:2364 (804 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MSInitImageButton
 * size-source: 804, not the census 795. The trailing 9 bytes are the dead second epilogue.
 *
 * Creates the window of an image button whose record `image_button_init_*` has filled in. The up
 * face at +0x22 is loaded from picture `res_up` and the down face at +0x48 from `res_down` --
 * through `GRPort_LoadPicture`, or 1068:363f when byte +0x72 is set -- and when both are the
 * same picture the down face is remade from the up face two pixels down and right over the colour
 * at DGROUP:0x337e, the same pressed look `sprite_button_init_1060_91e5` builds. Both faces take
 * the palette of the control's window. The control window is then created as an "MSControlClass"
 * child (WS_CHILD | WS_BORDER | WS_TABSTOP, and WS_VISIBLE when asked) the size of the down face
 * at (x, y) in the owner's window, given a control block of kind 5, and subclassed to 1070:2782.
 * The window handle is the answer.
 *
 * Four `TextColor` calls -- 0xff, 0xf8, 0xf8, 0xff -- set colours nothing here uses.
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
class SButton : public DlgControl { public: short enabled; long action; short colour; SButton(); ~SButton(); };
class SImageButton : public DlgControl { public: long on_click; long on_rclick; GRPort up; GRPort down; void *ctable; char reinit_faces; char pad_73; SImageButton(); ~SImageButton(); };
class SRegion : public DlgControl { public: long on_enter; long on_leave; long on_click; long on_press; long on_dblclk; SRegion(); ~SRegion(); };
class SScrollBar : public DlgControl { public: long on_change; long on_track; short page; short f24; short pos; SScrollBar(); ~SScrollBar(); };
class SCheckBox : public DlgControl { public: long on_change; short checked; short enabled; SCheckBox(); ~SCheckBox(); };
class SRadioSet : public DlgControl { public: long on_change; long on_dblclk; long on_rbutton_down; short count; short cols; short cur_item; short hmem; void *items; SRadioSet(); ~SRadioSet(); };
class SEditBox : public DlgControl { public: long on_change; long key_filter; char multiline; char pad_23; long f24; SEditBox(); ~SEditBox(); };
class SListBox : public DlgControl { public: short count; long f1c; SListBox(); ~SListBox(); };
#include "records.h"

struct logpal { unsigned short version; unsigned short count; PALETTEENTRY entries[1]; };
extern int GRPort_LoadPicture(GRPort *port, int res, int w, int h, struct logpal *pal);
extern void load_picture_reinit(void *port, int res, int first, int count, void *pal);
extern void GRPort_FillColor_all(GRPort *p, int color);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
typedef struct { char pal[0x404]; unsigned hpal; short seed; short steps, first, count; short steps2, first2, count2; unsigned char r, g, b; char f417; short hsrc_a, hsrc_b, hsrc; } CTable;   /* 0x41e bytes: the palette, its handle and the cycling state */
extern void GRPort_ColorSync(GRPort *port, CTable *table);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" int port_width_1070_98a3(void *port);
extern "C" int port_height_1070_98bc(void *port);
extern "C" void *ctl_window_1070_96a8(void *ctl);
extern "C" void *get_ctable_1070_97b7(void *c);

extern "C" void *get_win_1070_95f0(void *c);
extern "C" void *InitCtrlBlock(unsigned hwnd, void *obj);

unsigned MSInitImageButton(register int x, register int y, SImageButton *obj, int res_down,
                               int res_up, int pal_first, int pal_count, int visible)
{
    unsigned hwnd;
    SImageButton *button;
    CtrlBlock *blk;
    RECT dst;
    RECT src;
    RECT r;
    long style;

    button = obj;
    if (button->reinit_faces == 0) {
        GRPort_LoadPicture(&button->down, res_down, pal_first, pal_count, (struct logpal *)button->ctable);
        GRPort_LoadPicture(&button->up, res_up, pal_first, pal_count, (struct logpal *)button->ctable);
    } else {
        load_picture_reinit(&button->down, res_down, pal_first, pal_count, (struct logpal *)button->ctable);
        load_picture_reinit(&button->up, res_up, pal_first, pal_count, (struct logpal *)button->ctable);
    }
    MSDoAllTasks_1070_9837();
    SetRect(&r, x, y, port_width_1070_98a3(&button->down) + x - 1, port_height_1070_98bc(&button->down) + y - 1);
    OffsetRect(&r, -r.left, -r.top);
    if (res_down == res_up) {
        dst = r;
        SetRect(&dst, r.left, r.top, r.right - 2, r.bottom - 2);
        src = dst;
        OffsetRect(&dst, 2, 2);
        GRPort_FillColor_all(&button->down, g_draw_color);
        GRPort_CopyToPort(&button->up, &button->down, &src, &dst);
    }
    MSDoAllTasks_1070_9837();
    GRPort_ColorSync(&button->down, (CTable *)get_ctable_1070_97b7((Screen *)(ctl_window_1070_96a8(button))));
    GRPort_ColorSync(&button->up, (CTable *)get_ctable_1070_97b7((Screen *)(ctl_window_1070_96a8(button))));
    TextColor(0xff);
    TextColor(0xf8);
    TextColor(0xf8);
    TextColor(0xff);
    SetRect(&r, x, y, port_width_1070_98a3(&button->down) + x, port_height_1070_98bc(&button->down) + y);
    style = 0x40810000L;
    if (visible != 0)
        style |= 0x10000000L;
    hwnd = CreateWindow("MSControlClass", "", style, x, y, rect_width_1070_962a(&r),
                        rect_height_1070_9649(&r),
                        ((MSWin *)get_win_1070_95f0((Screen *)(ctl_window_1070_96a8(obj))))->hwnd,
                        0, g_hinstance, 0L);
    blk = (CtrlBlock *)InitCtrlBlock(hwnd, obj);
    blk->kind = 5;
    SetWindowLong(hwnd, -4, (long)(void *)image_button_proc);
    return hwnd;
}
