/* PORT: this file's own flattened PediaWin replaced by PediaWin_ctor.c's class (list asserted at 0x83e): its
 * int fields are 4 bytes here and it was not packed, so every member after tile_w was misplaced (W7). */
/* ColonyWin_colony_hammer_change: candidate for 1010:6f3a (1041 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_colony_hammer_change
 * name-source: sibling-build
 *
 * The colony window's "what to build" menu. It lists what the colony can build, one
 * `ColonyWin_build_list_add` line per item, under the heading word at SEG20:0xd98a, with the item
 * under construction pre-selected. Up to 22 items fit one menu; more are split into two pages of 16
 * with page-turn lines (ids 0x62 back, 0x63 forward, both labelled with the word at SEG20:0xd98e),
 * and turning the page rebuilds the menu.
 *
 * The colony's item (+0x94) becomes the answer less one, with bit 7 of +0x1c cleared -- so answer 0
 * stores -1, building nothing, and it alone hides the cost display (+0xa08 cleared, control +0x950
 * hidden) where every other answer shows it. When the menu reports its
 * alternate-click flag (`Popup_help_me`), the item is not chosen: its Colonopedia article
 * opens instead -- buildings as category 4, units as category 1 -- and the menu is rebuilt.
 *
 * THE COLONOPEDIA PATH LEAKS THE MENU. It jumps back to the top of the loop, which allocates a new
 * `Popup`, without deleting the one it came from; every other path deletes it. Each such look-up
 * costs 0x3dc bytes of the heap until the game exits.
 *
 * THE CLASS DECLARATION DECIDES THE `delete`. The menu is `new Popup` / `delete`, and the shipped
 * code copies the pointer into a temporary, null-tests it, moves the unwinder headcount by 4 and
 * brackets the destructor call with exception-state stores. That is what Borland writes for a class
 * with three destructible sub-objects -- Popup's GRPort at +8 and Sprites at +0x2e and +0x4e, as
 * `Popup_ctor` builds them (rule 49's `1 + 3`). Declared as a flat block of bytes, the same source
 * calls the destructor directly and the frame is four bytes smaller.
 *
 * The count test is `if (count <= 14) {} else if (count > 22)`: an empty then-arm, which is where
 * the shipped JMP over the second test comes from. The window's timer at +0xa10 is reset and control
 * +0x972 shown first, and DGROUP:0xa04 guards re-entry.
 *
 * RENAMED 2026-09-27 from colony_choose_build: CONST PROPOSAL. Mac ColonyWin::colony_hammer_change(short), CODE 30 0x01b8, 1072 bytes. Callers: Mac KeyClick and ColonyHammerChange; Windows ColonyWin_KeyClick and colony_choose_build_clicked (= ColonyHammerChange, below). Callees: ResetTimer, show/enable controls, set_colony, count_eligible_builds, Popup ctor, set_width, strings x3, string, item x2, nth_eligible_build, build_list_add (= build_menu_item), hilite x3, set_active x3, exec, help_me, get_production_class, ShowModal x2, Popup dtor, colony_change_mode. 6f3a's callees are the same (PediaWin_ShowModal x2 for ShowModal, popup_run_thunk for exec). Arity: this+short against (ColonyWin *w, int unused1). Layout: after build_list_add on both, and before the two button thunks and colony_hammer_buy. Size 1072 against 1041. The Mac spelling is lower-case, as in the already-adopted ColonyWin_colony_hammer_buy. The file is C++ with an extern "C" definition. Apply colony_choose_build_clicked -> ColonyHammerChange with it: the old name is a prefix of that file's name.
 */

#include "colonization.h"

#include "labels.h"
#include "w16types.h"
#include "records.h"
#include "classes.h"
#include <stddef.h>
class DlgControl;
template <class T> class LinkedList : public VList { public: LinkedList(); };
class PediaWin : public ColChild { public: short category; SImageButton button; SScrollBar bar; GRPort port; LinkedList<PediaEntry> list; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; PediaWin(); ~PediaWin(); };
typedef char pedia_list_at_0x83e[(offsetof(PediaWin, list) == 0x83e) ? 1 : -1];
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, ColonyWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









#pragma option -a

#pragma option -a-
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;                   /* SEG20:0x5ca */

extern char g_pedia_win;                /* SEG20:0x387c, declared */

extern void set_colony(int colony);
extern int count_eligible_builds(void);
extern int nth_eligible_build(int nth);
extern void Popup_set_width(Popup *popup, int width);
extern char *strings(int n);
extern "C" void Popup_string(Popup *popup, char *s);   /* PORT: C linkage, as defined (W2) */
extern "C" void Popup_item(Popup *popup, char *s, int id);   /* PORT: C linkage, as defined (W2) */
extern void Popup_hilite(Popup *o, int key, int on);
extern void Popup_set_active(Popup *popup, int key);
extern int Popup_help_me(Popup *popup);
extern int get_production_class(int item, int *idx);
extern "C" void PediaWin_ShowModal(void *pedia, int category, int type, void *parent);   /* PORT: C linkage, as defined (W2) */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void MSShowControl_1010_e574(void *control);
extern "C" void MSHideControl_1010_e54d(void *control);
extern "C" void win_disable_1010_e459(void *o);
extern "C" void win_enable_1010_e4d4(void *o);

extern "C" void ColonyWin_build_list_add(ColonyWin *w, Popup *p, int item);

extern "C" void colony_change_mode(void);

extern "C" void ColonyWin_colony_hammer_change(ColonyWin *w, int unused1)
{
    int count;
    int pages = 1;
    int per_page;
    int base;
    int item;
    int again = 1;
    int choice;
    int kind;
    int preselected = 0;
    Popup *popup;
    register int k;
    register int page;

    if (w->timer != -1) {
        ResetTimer(w->timer);
        w->timer = -1;
    }
    MSShowControl_1010_e574(&w->b[1]);
    if (g_choose_busy != 0)
        return;
    g_choose_busy = 1;
    win_disable_1010_e459(&g_colony_win_rec);
    set_colony(w->colony);
    count = count_eligible_builds();
    per_page = count;
    if (count <= 0xe) {
    } else if (count > 0x16) {
        pages = 2;
        per_page = 0x10;
    }
    page = 0;
    do {
top:
        popup = new Popup(0x800);
        Popup_set_width(popup, 0x1f4);
        popup->flags |= 1;
        Popup_string(popup, strings(g_ctitle_word[CTITLE_SELECT_AN_ITEM_TO_BUILD]));
        if (page > 0)
            Popup_item(popup, strings(g_ctitle_word[CTITLE_MORE]), 0x62);
        base = per_page * page;
        for (k = 0; k < per_page; k++) {
            item = nth_eligible_build(base + k);
            if (item >= -1) {
                ColonyWin_build_list_add(w, popup, item);
                if (g_colonies[w->colony].item == item) {
                    Popup_hilite(popup, item, 1);
                    Popup_set_active(popup, item + 1);
                    preselected = 1;
                }
            }
        }
        if (pages - 1 > page) {
            Popup_item(popup, strings(g_ctitle_word[CTITLE_MORE]), 0x63);
            if (preselected == 0) {
                Popup_set_active(popup, 0x63);
                Popup_hilite(popup, 0x63, 1);
                preselected = 1;
            }
        }
        if (preselected == 0 && page != 0) {
            Popup_set_active(popup, 0x62);
            Popup_hilite(popup, 0x62, 1);
        }
        popup->flags |= 0x40;
        choice = popup_run_thunk_1010_e4b1(popup) - 1;
        switch (choice) {
        case 0x62:
            page--;
            break;
        case 0x63:
            page++;
            break;
        default:
            again = 0;
            if (choice >= 0) {
                if (choice == 0) {
                    w->buy_shown = 0;
                    MSHideControl_1010_e54d(&w->b[0]);
                } else {
                    w->buy_shown = 1;
                    MSShowControl_1010_e574(&w->b[0]);
                }
                if (Popup_help_me(popup) != 0) {
                    kind = choice - 1;
                    kind = get_production_class(kind, &item);
                    if (kind == 1)
                        PediaWin_ShowModal((PediaWin *)&g_pedia_win, 4, item, &g_colony_win_rec);
                    else if (kind == 2)
                        PediaWin_ShowModal((PediaWin *)&g_pedia_win, 1, item, &g_colony_win_rec);
                    goto top;
                }
                g_colonies[w->colony].item = choice - 1;
                g_colonies[w->colony].status &= 0x7f;
            }
            break;
        }
        delete popup;
    } while (again != 0);
    g_choose_busy = 0;
    win_disable_1010_e459((Screen *)&g_main_win);
    win_enable_1010_e4d4(&g_colony_win_rec);
    win_show_and_focus_1010_e4ff(&g_colony_win_rec);
    colony_change_mode();
}
