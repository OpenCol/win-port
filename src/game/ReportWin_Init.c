/* PORT: this file's own declarations of ColChild_InitCWin_nc_parent, MSInitImageButton, get_ctable_1058_c3d9, image_button_init_1058_ba40, win_set_drag_height_1078_2aef, win_set_icon_1078_2832 removed: the header declares them from the definition (W1 step 3). */
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
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, ReportWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-







class SButton;

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;   /* SEG20:0x5ca */

extern int ColChild_IsSmallDesktop(void *w);
struct logpal;
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *ctable);

extern void DialogWin_AddControl(void *win, DlgControl *ctl);
extern void ColChild_InitCWin(ColChild *o, char *title, int style, int x, int y, int w, int h, char save);

extern void ReportWin_DrawReport(void);
extern void ReportWin_OnOKClick(void);

extern "C" int port_width_1058_bcfb(GRPort *port);

extern "C" int port_height_1058_bd14(GRPort *port);


extern "C" void set_keyboard_nav_1058_b992(DialogWin *w, char keyboard_nav);

extern "C" void win_set_icon_1058_b9ab(DialogWin *w, int icon);

extern "C" void dlg_control_init_1058_c3f6(DlgControl *ctl, void *win, int kind, int id, RECT *r);


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

