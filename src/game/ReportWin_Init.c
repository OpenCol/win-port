/* ReportWin_Init: candidate for 1058:0021 (507 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_Init
 *
 * Builds the report window: flag byte 0 and cursor 0x83 on its window, the three button
 * pointers and the paging words cleared, the full-screen flag (forced on a 640-wide desktop),
 * the game record pointer (the main record when none is given), then the "Reports" window
 * (style 4, 640x480; the MDI form with the record when full-screen), the report rectangle
 * published at DGROUP:0x2664, the close and repaint handlers, and on larger desktops the
 * Done image button (id 0x65, picture 0xce, palette range 0x28/0x60) at (0, 0) -- the
 * bottom-right rectangle built just before it is passed to nothing; otherwise the window's
 * drag height is set to 0. The module's wrappers are defined here because the shipped calls to
 * them are the compiler's own `push cs / call `.
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
class SButton; class ReportWin : public ColChild { public: short kind; SImageButton image; short nation; short page; short view; short score; unsigned char modal; char pad_7f9; void *rec; int cell_x[29]; int cell_y[29]; int counts[29]; SButton *prev; SButton *next; SButton *extra_btn; ReportWin(); ~ReportWin(); };

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;   /* SEG20:0x5ca */

extern int ColChild_IsSmallDesktop(void *w);
extern void win_set_icon_1078_2832(void *win, int icon);
extern void win_set_drag_height_1078_2aef(void *win, int drag_height);
struct logpal;
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *ctable);

extern void DialogWin_AddControl(void *win, DlgControl *ctl);
extern unsigned MSInitImageButton(int x, int y, SImageButton *b, int res, int res2, int w, int h, int a);
extern void ColChild_InitCWin(ColChild *o, char *title, int style, int x, int y, int w, int h, char save);
extern void ColChild_InitCWin_nc_parent(void *o, char *title, int style, int x, int y, int w, int h, int save, int noclose, void *rec);

extern void ReportWin_DrawReport(void);
extern void ReportWin_OnOKClick(void);

extern "C" int port_width_1058_bcfb(GRPort *port);

extern "C" int port_height_1058_bd14(GRPort *port);

extern "C" void *get_ctable_1058_c3d9(DialogWin *w);

extern "C" void set_keyboard_nav_1058_b992(DialogWin *w, char keyboard_nav);

extern "C" void win_set_icon_1058_b9ab(DialogWin *w, int icon);

extern "C" void dlg_control_init_1058_c3f6(DlgControl *ctl, void *win, int kind, int id, RECT *r);

extern "C" void image_button_init_1058_ba40(SImageButton *b, DialogWin *win, int id, int x, int y, int res, int res2, int w, int h);

extern "C" void set_control_action_1058_bb68(SImageButton *button, void *h);

extern "C" void win_set_drag_height_1058_bb82(DialogWin *w, int drag_height);

extern "C" void *set_on_close_1058_b9cf(EventTable *table, void *h);

extern "C" void set_repaint_handler_1058_ba08(ColChild *o, void *h);

void ReportWin_Init(ReportWin *o, char flag, void *parent)
{
    RECT rc;

    if (ColChild_IsSmallDesktop(o) != 0)
        flag = 1;
    set_keyboard_nav_1058_b992(&(DialogWin &)*o, 0);
    win_set_icon_1058_b9ab(&(DialogWin &)*o, 0x83);
    o->prev = 0;
    o->next = 0;
    o->extra_btn = 0;
    o->view = 0;
    o->page = 0;
    o->score = 0;
    o->modal = flag;
    if (parent == 0)
        parent = &g_main_win;
    o->rec = parent;
    o->kind = -1;
    if (o->modal == 0)
        ColChild_InitCWin((ColChild *)o, "Reports", 4, 0, 0, 640, 480, 0);
    else
        ColChild_InitCWin_nc_parent(o, "Reports", 4, 0, 0, 640, 480, 0, 1, parent != 0 ? parent : (void *)&g_main_win);
    g_report_rect = &o->rect;
    set_on_close_1058_b9cf(&(EventTable &)*o, (void *)ReportWin_Close);
    set_repaint_handler_1058_ba08(o, (void *)ReportWin_DrawReport);
    if (ColChild_IsSmallDesktop(o) == 0) {
        SetRect(&rc, 0x208, o->rect.top + 0x1c2, 0x26c, o->rect.top + 0x1db);
        image_button_init_1058_ba40(&o->image, &(DialogWin &)*o, 0x65, 0, 0, 0xce, 0xce, 0x28, 0x60);
        set_control_action_1058_bb68(&o->image, (void *)ReportWin_OnOKClick);
    } else
        win_set_drag_height_1058_bb82(&(DialogWin &)*o, 0);
}

