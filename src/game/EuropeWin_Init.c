/* EuropeWin_Init: candidate for 1048:863f (706 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_Init
 *
 * Builds the Europe window: the "Europe" window (style 8, 640x454), flag byte 0, the mouse,
 * close, key and accelerator handlers, the port filled with the background colour, the
 * scrolling region (id 0x80) at the bottom right with `EuropeWin_OnOKClick` (the Done action) as its click handler, the
 * ~Recruit / ~Purchase / ~Train buttons (ids 0xc8..0xca, 30 apart, face colour 0x7c) all
 * routed to EuropeWin_EuropeButtonClick, and the Done image button (id 0x65, picture 0xce, 40x96). The
 * module's wrappers are defined here because the shipped calls to them are the compiler's
 * own `push cs / call `; the window creation itself is a call TLINK converted.
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
class EuropeWin : public ColChild { public: short nation; SRegion region; SButton recruit; SButton train; SButton purchase; SImageButton image; GRPort port; EuropeWin(); ~EuropeWin(); };

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;   /* SEG20:0x5ca */

extern int ColChild_IsSmallDesktop(void *w);

extern void GRPort_FillColor_all(GRPort *p, int color);
extern short MSInitRegion(RECT *r, DlgControl *ctl);
extern short MSInitButton(RECT *rgn, SButton *obj, char *title, int visible);
extern void ColChild_InitCWin_style_nc(ColChild *o, char *title, int which, int x, int y, int w, int h, int style, char save, char noclose);
struct logpal;
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *ctable);

extern void DialogWin_AddControl(void *win, DlgControl *ctl);
extern unsigned MSInitImageButton(int x, int y, SImageButton *b, int res, int res2, int w, int h, int a);
extern void ColChild_InitCWin(ColChild *o, char *title, int style, int x, int y, int w, int h, char save);
extern void ColChild_InitCWin_nc_parent(void *o, char *title, int style, int x, int y, int w, int h, int save, int noclose, void *rec);

extern "C" int port_width_1048_d853(GRPort *port);

extern "C" int port_height_1048_d86c(GRPort *port);

extern "C" void *get_ctable_1048_e5f6(DialogWin *w);

extern "C" void set_keyboard_nav_1048_deb3(DialogWin *w, char keyboard_nav);

extern "C" void dlg_control_init_1048_e633(DlgControl *ctl, void *win, int kind, int id, RECT *r);

extern "C" void image_button_init_1048_d532(SImageButton *b, DialogWin *win, int id, int x, int y, int res, int res2, int w, int h);

extern "C" void set_control_action_1048_d65a(SImageButton *button, void *h);

extern void EuropeWin_MotherFuckinAsciiParse(void);
extern void EuropeWin_EuropeKeyParse(int key);
extern void EuropeWin_OnOKClick(int unused1);
extern void EuropeWin_EuropeButtonClick(void);

extern "C" void ColChild_InitCWin_nc(ColChild *o, char *title, int which, int x, int y, int w, int h, int save, int noclose);

extern "C" void sregion_init_1048_dfb0(SRegion *b, DialogWin *win, int id, RECT *r);

extern "C" void set_region_click_1048_e033(SRegion *b, void *h);

extern "C" void ctl_init_button_1048_e831(SButton *button, DialogWin *win, int id, RECT *r, char *title);

extern "C" void ctl_init_button_1048_e04d(SButton *button, DialogWin *win, int id, RECT *r, int colour, char *title);

extern "C" void set_button_action_1048_e087(SButton *button, void *h);

extern "C" void *set_on_lbutton_down_1048_decc(EventTable *table, void *h);

extern "C" void *set_on_rbutton_down_1048_df05(EventTable *table, void *h);

extern "C" void *set_on_mousemove_1048_da35(EventTable *table, void *h);

extern "C" void *set_on_close_1048_df3e(EventTable *table, void *h);

extern "C" void *set_on_char(EventTable *table, void *h);

extern "C" void *set_on_keydown_1048_dceb(EventTable *table, void *h);

void EuropeWin_Init(EuropeWin *o)
{
    RECT rc;

    o->nation = -1;
    ColChild_InitCWin_nc(o, "Europe", 8, 0, 0, 640, 454, 0, 1);
    set_keyboard_nav_1048_deb3(&(DialogWin &)*o, 0);
    set_on_lbutton_down_1048_decc((EventTable *)(&(EventTable &)*o), (void *)EuropeWin_LeftMouseClick);
    set_on_rbutton_down_1048_df05((EventTable *)(&(EventTable &)*o), (void *)EuropeWin_RightMouseClick);
    set_on_mousemove_1048_da35((EventTable *)(&(EventTable &)*o), (void *)EuropeWin_MouseMove);
    set_on_close_1048_df3e((EventTable *)(&(EventTable &)*o), (void *)EuropeWin_Close);
    set_on_char((EventTable *)(&(EventTable &)*o), (void *)EuropeWin_MotherFuckinAsciiParse);
    set_on_keydown_1048_dceb((EventTable *)(&(EventTable &)*o), (void *)EuropeWin_EuropeKeyParse);
    GRPort_FillColor_all((GRPort *)o, g_draw_color);
    SetRect(&rc, o->rect.left + 0x262, o->rect.top + 0x194, o->rect.left + 0x280, o->rect.top + 0x1c6);
    sregion_init_1048_dfb0(&o->region, &(DialogWin &)*o, 0x80, &rc);
    set_region_click_1048_e033(&o->region, (void *)EuropeWin_OnOKClick);
    SetRect(&rc, o->rect.left + 0x212, o->rect.top + 0xae, o->rect.left + 0x27b, o->rect.top + 0xc7);
    ctl_init_button_1048_e04d(&o->recruit, &(DialogWin &)*o, 0xc8, &rc, 0x7c, "~Recruit");
    set_button_action_1048_e087(&o->recruit, (void *)EuropeWin_EuropeButtonClick);
    OffsetRect(&rc, 0, 30);
    ctl_init_button_1048_e04d(&o->purchase, &(DialogWin &)*o, 0xc9, &rc, 0x7c, "~Purchase");
    set_button_action_1048_e087(&o->purchase, (void *)EuropeWin_EuropeButtonClick);
    OffsetRect(&rc, 0, 30);
    ctl_init_button_1048_e04d(&o->train, &(DialogWin &)*o, 0xca, &rc, 0x7c, "~Train");
    set_button_action_1048_e087(&o->train, (void *)EuropeWin_EuropeButtonClick);
    image_button_init_1048_d532(&o->image, &(DialogWin &)*o, 0x65, 0, 0, 0xce, 0xce, 0x28, 0x60);
    set_control_action_1048_d65a(&o->image, (void *)EuropeWin_OnOKClick);
}

