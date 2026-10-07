/* PORT: this file's own declarations of ColChild_IsSmallDesktop, DialogWin_AddControl, MSInitButton, ctl_init_button_1048_e831 removed: the header declares them from the definition (W1 step 3). */
/* PORT: this file's own declarations of ColChild_InitCWin_nc_parent, EuropeWin_EuropeButtonClick, EuropeWin_MotherFuckinAsciiParse, MSInitImageButton, MSInitRegion, ctl_init_button_1048_e04d, get_ctable_1048_e5f6, image_button_init_1048_d532, sregion_init_1048_dfb0 removed: the header declares them from the definition (W1 step 3). */
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
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, EuropeWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-










typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;   /* SEG20:0x5ca */


extern void GRPort_FillColor_all(GRPort *p, int color);
extern void ColChild_InitCWin_style_nc(ColChild *o, char *title, int which, int x, int y, int w, int h, int style, char save, char noclose);
struct logpal;
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *ctable);

extern void ColChild_InitCWin(ColChild *o, char *title, int style, int x, int y, int w, int h, char save);

extern "C" int port_width_1048_d853(GRPort *port);

extern "C" int port_height_1048_d86c(GRPort *port);


extern "C" void set_keyboard_nav_1048_deb3(DialogWin *w, char keyboard_nav);

extern "C" void dlg_control_init_1048_e633(DlgControl *ctl, void *win, int kind, int id, RECT *r);


extern "C" void set_control_action_1048_d65a(SImageButton *button, void *h);

extern void EuropeWin_EuropeKeyParse(int key);
extern void EuropeWin_OnOKClick(int unused1);

extern "C" void ColChild_InitCWin_nc(ColChild *o, char *title, int which, int x, int y, int w, int h, int save, int noclose);


extern "C" void set_region_click_1048_e033(SRegion *b, void *h);



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

