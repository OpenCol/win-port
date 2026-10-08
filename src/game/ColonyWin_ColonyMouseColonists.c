/* PORT: this file's own flattened PediaWin replaced by PediaWin_ctor.c's class (list asserted at 0x83e): its
 * int fields are 4 bytes here and it was not packed, so every member after tile_w was misplaced (W7). */
/* ColonyWin_ColonyMouseColonists: candidate for 1010:6459 (726 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_ColonyMouseColonists
 * name-source: sibling-build
 * size-source: 726, not the census 724. The census stops two bytes short of the closing RETF; what
 * follows up to colony_click_ship (1010:674c) is data.
 *
 * Panel 0 of the colony window, a row of colonists picked by x alone (`WhichColonist`):
 * a click, and the drop target for a colonist dragged from panel 1 or 2.
 *
 *   - As a DROP (panel word 6) it only opens the colonist-job popup (`colony_select_profession`), when both DGROUP:0x9e0 and
 *     0x9e6 are set.
 *   - A click on a colonist that is not already the row's selection (DGROUP:0x4bbe, with 0xc06 at
 *     1) makes it both the row's selection and the colony's selected colonist (0x4bbc), and moves
 *     the grid cursor to its work square if `colonist_find` finds one. The
 *     previous selection is remembered.
 *   - Outside Colonopedia mode, after 5 ticks a still-held button drags the colonist's portrait
 *     and offers the drop to panels 2, 1 and 0 under panel word 6. The distance test is
 *     LOPSIDED: a drop counts as a drag when it lands more than 5 pixels left, or more than 10
 *     right, up or down, where panel 1's version uses 10 all round.
 *   - A plain click restores the remembered selection and, when `g_colony_drop_active` is set and the button came
 *     up inside panel 0, selects the row colonist or opens the colonist-job popup (`colony_select_profession`) if it already
 *     was selected. In Colonopedia mode it opens the colonist's specialty article (category 3),
 *     reading specialty 0x1c as 0x13: 0x13 is JOB19, "FREE COLONISTS", and 0x1c the "none" the
 *     specialty table uses (ColonyWin_ColonyClickUnits).
 *
 * Every path ends in `colony_change_selected`. `g_selected_colonist = g_row_colonist = c` is one
 * chained store, the value carried in DX and then AX. The drop arm's two exits are `goto done`
 * on both sides of the distance test (the dead jump between them).
 *
 * RENAMED 2026-09-27 from colony_row_click: BEYOND THE PROPOSALS. Mac ColonyWin::ColonyMouseColonists(short, short), CODE 13 0x318a, 674 bytes. Callers (bsr): ColonyMouseParse, ColonyMouseMap, ColonyMouseBuildings, itself. Windows: colony_panel_rclick, ColonyWin_ColonyMouseMap, colony_building_click, itself, and colony_panel_dispatch. Callees: ColonyMouseArea x2, set_colony, WhichColonist, colonist_find, colony_select_profession x2, colony_change_selected x3, MSWaitTicks, LButtonDownNow, get_icon, DragIcon, ColonyMouseBuildings, ColonyMouseMap, get_specialty, ShowModal, reset_colony. 6459's callees are that list item for item. Arity: this+2 against (w, x, y). Layout: after ColonyMouseUnits, before colony_click_ship, on both. Size 674 against 726. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"
#include <stddef.h>
class DlgControl;
template <class T> class LinkedList : public VList { public: LinkedList(); };
class PediaWin : public ColChild { public: short category; SImageButton button; SScrollBar bar; GRPort port; LinkedList<PediaEntry> list; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; PediaWin(); ~PediaWin(); };
typedef char pedia_list_at_0x83e[(sizeof(void *) != 4 || offsetof(PediaWin, list) == 0x83e) ? 1 : -1];   /* PORT: the game's offset where a pointer is 4 bytes (W9) */
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, ColonyWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

extern char g_pedia_win;               /* SEG20:0x387c, declared */
extern void set_colony(int colony);
extern void colony_select_profession(int unit_roles_only);
extern "C" int colonist_find(int id, int *col, int *row);   /* PORT: C linkage, as defined (W2) */
extern "C" int Screen_LButtonDownNow(void *w);   /* PORT: C linkage, as defined (W2) */
extern int get_icon(int colonist);
extern "C" void ColChild_DragIcon(ColChild *w, void *sprite, int x, int y, int *rx, int *ry);   /* PORT: C linkage, as defined (W2) */
extern int get_specialty(int u);
extern "C" void PediaWin_ShowModal(void *pedia, int category, int type, void *parent);   /* PORT: C linkage, as defined (W2) */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int ColonyMouseArea(int x, int y);
extern "C" int WhichColonist(int x, int y);
extern "C" void colony_change_selected(void);

extern "C" void ColonyWin_ColonyMouseBuildings(ColonyWin *w, int x, int y);
extern "C" void ColonyWin_ColonyMouseMap(ColonyWin *w, int x, int y);
extern "C" void reset_colony(void);

void ColonyWin_ColonyMouseColonists(ColonyWin *w, int x, int y)
{
    int colonist;
    int col;
    int row;
    int ox;
    int oy;
    register int prev = -1;
    register int kind;

    if (ColonyMouseArea(x, y) != 0)
        return;
    set_colony(w->colony);
    colonist = WhichColonist(x, y);
    if (w->panel == 6) {
        if (g_colony_drop_active != 0 && g_hit_panel != 0)
            colony_select_profession(0);
    } else {
        if (g_row_colonist != colonist || g_panel_mode != 1) {
            g_panel_mode = 1;
            prev = g_selected_colonist;
            g_selected_colonist = g_row_colonist = colonist;
            if (colonist_find(g_row_colonist, &col, &row) != 0) {
                g_cell_col = col;
                g_cell_row = row;
            }
            colony_change_selected();
            if (g_colony_clicked == 0)
                g_colony_drag_armed = 0;
        }
        if (g_pedia_mode == 0) {
            MSWaitTicks_1010_e5f6(5);
            if (Screen_LButtonDownNow(&(DialogWin &)*w) != 0 && g_colony_drag_armed != 0) {
                ox = x;
                oy = y;
                ColChild_DragIcon((ColChild *)w, &g_icon_sprites[get_icon(g_row_colonist)], x, y, &x, &y);
                if (ox - 5 > x || ox + 10 < x || oy - 10 > y || oy + 10 < y) {
                    w->panel = 6;
                    g_9e2 = 0;
                    g_colony_drop_active = 1;
                    g_colony_clicked = 0;
                    ColonyWin_ColonyMouseBuildings(w, x, y);
                    ColonyWin_ColonyMouseMap(w, x, y);
                    ColonyWin_ColonyMouseColonists(w, x, y);
                    w->panel = 0;
                    g_9e2 = 1;
                    g_colony_clicked = 1;
                    goto done;
                } else
                    goto done;
            }
        }
        if (prev != -1)
            g_selected_colonist = prev;
        if (g_colony_drop_active != 0 && ColonyMouseArea(x, y) == 0) {
            if (g_row_colonist != g_selected_colonist)
                g_selected_colonist = g_row_colonist;
            else if (g_pedia_mode == 0)
                colony_select_profession(0);
            colony_change_selected();
            if (g_pedia_mode != 0) {
                kind = get_specialty(g_selected_colonist);
                if (kind == 0x1c)
                    kind = 0x13;
                PediaWin_ShowModal((PediaWin *)&g_pedia_win, 3, kind, &(DialogWin &)*w);
                reset_colony();
            }
        }
    }
done:
    colony_change_selected();
}
