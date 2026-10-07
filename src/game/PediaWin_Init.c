/* PORT: this file's own declarations of ColChild_InitCWin_nc_parent, MSCenterWindow, MSInitImageButton, get_ctable_1060_6b75, image_button_init_1060_645c, win_set_icon_1078_2832 removed: the header declares them from the definition (W1 step 3). */
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
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SScrollBar, VListItem, VList removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-










#include "records.h"
template <class T> class LinkedList : public VList { public: LinkedList(); };
class PediaWin : public ColChild { public: short category; SImageButton button; SScrollBar bar; GRPort port; LinkedList<PediaEntry> list; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; PediaWin(); ~PediaWin(); };

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;   /* SEG20:0x5ca */

extern int ColChild_IsSmallDesktop(void *w);

extern void GRPort_FillColor_all(GRPort *p, int color);
extern void GameWin_ResizePort(GameWin *o, void *rect);
struct logpal;
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *ctable);

extern void DialogWin_AddControl(void *win, DlgControl *ctl);
extern void ColChild_InitCWin(ColChild *o, char *title, int style, int x, int y, int w, int h, char save);
extern void PediaWin_OnOKClick(void);

extern "C" void win_center_1060_60e7(DialogWin *w);

extern "C" int port_width_1060_65ca(GRPort *port);

extern "C" int port_height_1060_65e3(GRPort *port);


extern "C" void set_keyboard_nav_1060_615c(DialogWin *w, char keyboard_nav);

extern "C" void win_set_icon_1060_61b9(DialogWin *w, int icon);

extern "C" void dlg_control_init_1060_6b92(DlgControl *ctl, void *win, int kind, int id, RECT *r);


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

