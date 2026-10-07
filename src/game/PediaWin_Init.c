/* PediaWin_Init: candidate for 1060:15f1 (397 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: PediaWin_Init
 *
 * Builds the Colonizopedia window: the list and article state words reset (+0x84a/+0x77a/
 * +0x84c to -1, +0x852 = 1, the mode byte +0x857 from `flag`), flag byte 0 and cursor 0x82 on
 * its window, the "Colonizopedia" window (style 7, 600x400; the MDI form with the record
 * when `flag`), centred; in the plain form a 1x1 click rectangle is mapped and the port
 * resized through its virtual; the port filled with the background colour (DGROUP:0x337e)
 * and the Done image button (id 0x65, picture 0xce, 40x96) created with PediaWin_OnOKClick as
 * its handler. The module's wrappers are defined here because the shipped calls to them are
 * the compiler's own `push cs / call `.
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
class SScrollBar : public DlgControl { public: long on_change; long on_track; short page; short f24; short pos; SScrollBar(); ~SScrollBar(); };
class VListItem { public: void *value; VListItem *next; VListItem *prev; VListItem(); ~VListItem(); };
class VList { public: VListItem *first; VListItem *last; VListItem *cur; VList(); ~VList(); };
#include "records.h"
template <class T> class LinkedList : public VList { public: LinkedList(); };
class PediaWin : public ColChild { public: short category; SImageButton button; SScrollBar bar; GRPort port; LinkedList<PediaEntry> list; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; PediaWin(); ~PediaWin(); };

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;   /* SEG20:0x5ca */

extern int ColChild_IsSmallDesktop(void *w);
extern void win_set_icon_1078_2832(void *win, int icon);
extern void MSCenterWindow(void *win, int x, int y);

extern void GRPort_FillColor_all(GRPort *p, int color);
extern void GameWin_ResizePort(GameWin *o, void *rect);
struct logpal;
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *ctable);

extern void DialogWin_AddControl(void *win, DlgControl *ctl);
extern unsigned MSInitImageButton(int x, int y, SImageButton *b, int res, int res2, int w, int h, int a);
extern void ColChild_InitCWin(ColChild *o, char *title, int style, int x, int y, int w, int h, char save);
extern void ColChild_InitCWin_nc_parent(void *o, char *title, int style, int x, int y, int w, int h, int save, int noclose, void *rec);
extern void PediaWin_OnOKClick(void);

extern "C" void win_center_1060_60e7(DialogWin *w);

extern "C" int port_width_1060_65ca(GRPort *port);

extern "C" int port_height_1060_65e3(GRPort *port);

extern "C" void *get_ctable_1060_6b75(DialogWin *w);

extern "C" void set_keyboard_nav_1060_615c(DialogWin *w, char keyboard_nav);

extern "C" void win_set_icon_1060_61b9(DialogWin *w, int icon);

extern "C" void dlg_control_init_1060_6b92(DlgControl *ctl, void *win, int kind, int id, RECT *r);

extern "C" void image_button_init_1060_645c(SImageButton *b, DialogWin *win, int id, int x, int y, int res, int res2, int w, int h);

extern "C" void set_control_action_1060_6584(SImageButton *button, void *h);

void PediaWin_Init(PediaWin *o, int flag, EventWin *parent)
{
    RECT rc;

    o->category = o->kind = -1;
    o->sel = -1;
    o->scrollbar_built = 0;
    o->initialised = 1;
    o->modal = flag;
    o->count = 0;
    o->page = 1;
    set_keyboard_nav_1060_615c(&(DialogWin &)*o, 0);
    win_set_icon_1060_61b9(&(DialogWin &)*o, 0x82);
    if (flag == 0)
        ColChild_InitCWin((ColChild *)o, "Colonizopedia", 7, 0, 0, 600, 400, 0);
    else
        ColChild_InitCWin_nc_parent(o, "Colonizopedia", 7, 0, 0, 600, 400, 0, 1, parent);
    win_center_1060_60e7(&(DialogWin &)*o);
    if (flag == 0) {
        SetRect(&rc, 0, 0, 1, 1);
        GameWin_ResizePort((GameWin *)o, &rc);
        o->port.ResizePort(&rc);
    }
    GRPort_FillColor_all((GRPort *)o, g_draw_color);
    image_button_init_1060_645c(&o->button, &(DialogWin &)*o, 0x65, 0, 0, 0xce, 0xce, 0x28, 0x60);
    set_control_action_1060_6584(&o->button, (void *)PediaWin_OnOKClick);
}

