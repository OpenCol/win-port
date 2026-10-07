/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 92, 110, 111, 112 of the imported file. */
/* PediaWin_Show: candidate for 1060:1be8 (998 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: PediaWin_Show
 * name-source: sibling-build
 *
 * OPENS THE COLONIZOPEDIA at one article, from all six &Colonizopedia menu items (with -1 for the
 * family's index page) and from &Reports > Terrain Information. It is `PediaWin_ShowModal` (1060:17a4)
 * without the caller's window: the same parchment, the same scrollbar built once, the same list fill and
 * redraw, and the same three handler slots.
 *
 * THE DIFFERENCES, all at the ends:
 *  - the window is initialised by `PediaWin_Init_default` and only when +0x858 is CLEAR, where the
 *    extended form detaches the menu first and always initialises;
 *  - the parchment is cut 80 pixels shorter at the bottom, not 40;
 *  - with the category still -1 (nothing opened yet) the window is laid out to the parchment's size;
 *  - it does NOT run a modal loop: it sets the icon, restores the window if it is iconic, installs the
 *    handlers and returns, leaving the window up.
 *
 * Transferred from `matched/PediaWin_ShowModal.c` and graded RELOCATABLE on the first build.
 *
 * RENAMED 2026-09-26 from open_colonopedia: the Mac build's PediaWin::Show(short, short): this plus a category and an article, opening the Colonopedia at that entry. Its sibling PediaWin_ShowModal is left unnamed: its 12-byte block does not match PediaWin::Init(short, EventWin*)'s 10.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
template <class T> class LinkedList : public VList { public: LinkedList(); };
class PediaWin : public ColChild { public: short category; SImageButton button; SScrollBar bar; GRPort port; LinkedList<PediaEntry> list; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; PediaWin(); ~PediaWin(); };

struct logpal;
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *pal);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern void ColChild_ResizeCWin(PediaWin *o, int w, int h);
extern void PediaWin_FillStringList(PediaWin *w);
extern void PediaWin_DrawListArea(PediaWin *w);
extern void PediaWin_LeftMouseClick(int x, int y);
extern void PediaWin_KeyClick(int key);
extern void PediaWin_OnScroll(int page);
extern int PediaWin_Close(void);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" int MSIsWindowMinimized_of_1060_67e4(DialogWin *w);
extern "C" void MSRestoreWindow_of(DialogWin *w);
extern "C" int port_width_1060_65ca(void *port);
extern "C" int port_height_1060_65e3(void *port);

extern "C" void scrollbar_init(void *o, void *win, int id, RECT *r, long l);
extern "C" void set_control_action_1060_6685(void *bar, void *h);
extern "C" void MSHideControl_1060_669f(void *control);
extern "C" void MSSetSBRange(void *control, int a, int b);
extern "C" void MSSetSBValue(void *control, int pos);
extern "C" void MSFlushInput_1060_6713(void);

extern "C" void show_window_1060_612a(PediaWin *w);
extern "C" void *set_on_lbutton_down_1060_6203(EventTable *table, void *fn);
extern "C" void *set_on_close_1060_6726(EventTable *table, void *fn);
extern "C" void *set_on_keydown_after_1060_675f(EventTable *table, void *fn);
extern "C" void win_set_icon_1060_61b9(DialogWin *port, int icon);

void PediaWin_Show(PediaWin *o, int category, int article)
{
    GRPort port;
    CTable ct;
    RECT r;
    RECT rc;

    if (o->initialised == 0)
        PediaWin_Init_default(o);
    GRPort_LoadPicture(&port, 0xca, 0x10, 0x18, (struct logpal *)&ct);
    SetRect(&r, 0x14, 0x28, port_width_1060_65ca(&port) - 0x14, port_height_1060_65e3(&port) - 0x50);
    if (o->category == -1)
        ColChild_ResizeCWin(o, rect_width_1060_63b5(&r), rect_height_1060_63d4(&r));
    MSDoAllTasks_1060_6149();
    GRPort_CopyToPort(&port, (GRPort *)o, &r, &o->rect);
    SetRect(&rc, 0, 0, rect_width_1060_63b5(&r), rect_height_1060_63d4(&r));
    o->port.ResizePort(&rc);
    GRPort_CopyToPort(&port, &o->port, &r, &rc);
    GRPort_Init_wh_1060_65fc(&port);
    o->sel = -1;
    o->page = 1;
    o->wanted = article;
    o->category = o->kind = category;
    if (o->scrollbar_built == 0) {
        SetRect(&r, rect_width_1060_63b5(&o->rect) + o->rect.left - 0x36,
                o->rect.top + 0x29,
                rect_width_1060_63b5(&o->rect) + o->rect.left - 0x1d,
                rect_height_1060_63d4(&o->rect) + o->rect.top - 0x29);
        scrollbar_init(&o->bar, &(DialogWin &)*o, 0x66, &r, 1L);
        set_control_action_1060_6685(&o->bar, (void*)(PediaWin_OnScroll));
        MSHideControl_1060_669f(&o->bar);
        o->scrollbar_built = 1;
    }
    MSHideControl_1060_669f(&o->bar);
    PediaWin_FillStringList(o);
    o->pages = o->count / 16 + 1;
    if (o->count % 16 == 0)
        o->pages = o->pages - 1;
    o->page = 1;
    MSSetSBRange(&o->bar, 1, o->pages);
    MSSetSBValue(&o->bar, o->page);
    PediaWin_DrawListArea(o);
    copy_all_to_screen_1060_610c(o);
    win_set_icon_1060_61b9(&(DialogWin &)*o, 0x82);
    if (MSIsWindowMinimized_of_1060_67e4(&(DialogWin &)*o) != 0)
        MSRestoreWindow_of(&(DialogWin &)*o);
    MSFlushInput_1060_6713();
    set_on_lbutton_down_1060_6203(&(EventTable &)*o, (void*)(PediaWin_LeftMouseClick));
    set_on_close_1060_6726(&(EventTable &)*o, (void*)(PediaWin_Close));
    set_on_keydown_after_1060_675f(&(EventTable &)*o, (void*)(PediaWin_KeyClick));
    show_window_1060_612a(o);
    MSDoAllTasks_1060_6149();
}

