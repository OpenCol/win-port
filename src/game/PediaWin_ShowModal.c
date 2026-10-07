/* PediaWin_ShowModal: candidate for 1060:17a4 (1053 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: PediaWin_ShowModal
 * name-source: sibling-build
 *
 * OPENS THE COLONIZOPEDIA and runs it modally: `category` picks the list (it goes to +0x84a and +0x77a),
 * `article` is the entry to open at (+0x854), and `parent` is the window to give the focus back to. The
 * plain `PediaWin_Show` is this with `parent` null, in which case the game window at SEG20:0x5ca takes the
 * focus back. It has 22 call sites.
 *
 * SET-UP: the menu is detached while the window is up (+0x858), the window initialised and framed, and
 * the parchment (picture 0xca) copied in twice -- once into the window's own rectangle and once, at the
 * size its virtual `ResizePort` sets, into the page port at +0x818.
 *
 * THE SCROLLBAR at +0x7f0 is built ONCE, the first time the window is opened (+0x856): a 25-pixel strip
 * down the right-hand side, its callback `PediaWin_OnScroll`. The page count is `count / 16 + 1`, less one
 * when the count divides exactly, and the bar is set to page 1 and hidden. It is hidden again on every
 * open, and `PediaWin_DrawListArea` shows it when there is more than one page.
 *
 * The mouse, the "window is current" answer and the key handler go into the object's handler slots at
 * +0x33 before the modal loop, and the window's icon is 0x82.
 *
 * Spellings: `o->category = o->kind = category`, which is what keeps `category` out of a register; the page
 * decrement written `o->pages = o->pages - 1`; and the picture rectangle's corners in order.
 *
 * RENAMED 2026-09-27 from open_colonopedia_ex: Mac PediaWin::ShowModal(short, short, EventWin*), 1150 bytes, against 1053. Callers: 15 of 15 match (ColonyWin ColonyMouseBuildings, Colonists, Map, ShipsProper, Units and colony_hammer_change; EuropeWin EuropeMouseMarket, ShipsCargo, ShipsProper, Transit and Units; choose_freedom, colony_select_profession, europe_purchase and europe_recruit). Callees match: PediaWin_Init, DrawNonClient, LoadPicture, CopyToPort x2, MSSetSBRange, MSHideControl x2, FillStringList, MSSetSBValue, DrawListArea, Center, Modal. Layout: Mac PediaWin::Init(void), Init(short, EventWin*), ShowModal, Show, KeyClick, DrawTitle; Windows PediaWin_Init_default, PediaWin_Init, 17a4, PediaWin_Show, KeyClick, DrawTitle. Arity: this+3 = (pedia, category, article, parent). mac_bind: strong (align 13/0, votes 22/6).
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SScrollBar, VListItem, VList, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-










#include "records.h"
template <class T> class LinkedList : public VList { public: LinkedList(); };
class PediaWin : public ColChild { public: short category; SImageButton button; SScrollBar bar; GRPort port; LinkedList<PediaEntry> list; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; PediaWin(); ~PediaWin(); };


typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;             /* SEG20:0x5ca */

struct logpal;
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *pal);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern void ColChild_DrawNonClient(ColChild *w);
extern void ColChild_Center(ColChild *w);
extern void EventWin_Modal(EventWin *w);
extern void PediaWin_FillStringList(PediaWin *w);
extern void PediaWin_DrawListArea(PediaWin *w);
extern void PediaWin_LeftMouseClick(int x, int y);
extern void PediaWin_KeyClick(int key);
extern void PediaWin_OnScroll(int page);
extern int PediaWin_Close(void);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void win_release_1060_659e(PediaWin *w);
extern "C" void PediaWin_Init(PediaWin *o, int flag, void *rec);
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
extern "C" void win_enable_1060_6798(DialogWin *o);
extern "C" void win_set_focus(DialogWin *w);

void PediaWin_ShowModal(PediaWin *pedia, int category, int article, EventWin *parent)
{
    GRPort port;
    CTable ct;
    RECT r;
    RECT rc;

    if (pedia->initialised != 0)
        win_release_1060_659e(pedia);
    PediaWin_Init(pedia, 1, parent);
    ColChild_DrawNonClient((ColChild *)pedia);
    GRPort_LoadPicture(&port, 0xca, 0x10, 0x18, (struct logpal *)&ct);
    SetRect(&r, 0x14, 0x28, port_width_1060_65ca(&port) - 0x14, port_height_1060_65e3(&port) - 0x28);
    GRPort_CopyToPort(&port, (GRPort *)pedia, &r, &pedia->rect);
    SetRect(&rc, 0, 0, rect_width_1060_63b5(&r), rect_height_1060_63d4(&r));
    pedia->port.ResizePort(&rc);
    GRPort_CopyToPort(&port, &pedia->port, &r, &rc);
    GRPort_Init_wh_1060_65fc(&port);
    pedia->sel = -1;
    pedia->page = 1;
    pedia->wanted = article;
    pedia->category = pedia->kind = category;
    if (pedia->scrollbar_built == 0) {
        SetRect(&r, rect_width_1060_63b5(&pedia->rect) + pedia->rect.left - 0x36,
                pedia->rect.top + 0x29,
                rect_width_1060_63b5(&pedia->rect) + pedia->rect.left - 0x1d,
                rect_height_1060_63d4(&pedia->rect) + pedia->rect.top - 0x29);
        scrollbar_init(&pedia->bar, &(DialogWin &)*pedia, 0x66, &r, 1L);
        set_control_action_1060_6685(&pedia->bar, PediaWin_OnScroll);
        MSHideControl_1060_669f(&pedia->bar);
        pedia->scrollbar_built = 1;
    }
    MSHideControl_1060_669f(&pedia->bar);
    PediaWin_FillStringList(pedia);
    pedia->pages = pedia->count / 16 + 1;
    if (pedia->count % 16 == 0)
        pedia->pages = pedia->pages - 1;
    pedia->page = 1;
    MSSetSBRange(&pedia->bar, 1, pedia->pages);
    MSSetSBValue(&pedia->bar, pedia->page);
    PediaWin_DrawListArea(pedia);
    copy_all_to_screen_1060_610c(pedia);
    ColChild_Center((ColChild *)pedia);
    show_window_1060_612a(pedia);
    MSFlushInput_1060_6713();
    MSDoAllTasks_1060_6149();
    set_on_lbutton_down_1060_6203(&(EventTable &)*pedia, PediaWin_LeftMouseClick);
    set_on_close_1060_6726(&(EventTable &)*pedia, PediaWin_Close);
    set_on_keydown_after_1060_675f(&(EventTable &)*pedia, PediaWin_KeyClick);
    win_set_icon_1060_61b9(&(DialogWin &)*pedia, 0x82);
    EventWin_Modal((EventWin *)(&(DialogWin &)*pedia));
    win_release_1060_659e(pedia);
    pedia->initialised = 0;
    if (parent != 0) {
        win_enable_1060_6798((DialogWin *)parent);
        win_set_focus((DialogWin *)parent);
    } else {
        win_enable_1060_6798((DialogWin *)&g_main_win);
        win_set_focus((DialogWin *)&g_main_win);
    }
}

