/* PORT: this file's own flattened PediaWin replaced by PediaWin_ctor.c's class (list asserted at 0x83e): its
 * int fields are 4 bytes here and it was not packed, so every member after tile_w was misplaced (W7). */
/* EuropeWin_EuropeMouseTransit: candidate for 1048:3501 (1298 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_EuropeMouseTransit
 * name-source: sibling-build
 *
 * A CLICK OR A DROP ON ONE OF THE EUROPE SCREEN'S TWO VOYAGE LISTS (`EuropeMouseArea` 2 or 3),
 * the ships under way between Europe and the New World. The list is held by the unit squares off the
 * map at `nation - 28` and `nation - 24` (region 2) and `nation - 16` and `nation - 12` (region 3).
 * The market, cargo and dock click handlers pass drops on to it, and it calls itself for a drag that
 * ends elsewhere.
 *
 * BY THE DRAG STATE (DGROUP:0x1fc4):
 *   - 10: nothing;
 *   - 8: the selected ship asks to sail for the New World (`attempt_to_set_sail`), and the
 *     window is refreshed if that cleared DGROUP:0x4cd2;
 *   - 9, A SHIP DROPPED HERE from another list: its orders are cleared. Taking along the units after it
 *     in its chain while their size fits the free holds (stopping at the next ship), it is put on this
 *     voyage's square (`nation - 24` for region 2, `nation - 12` for 3). The Europe screen is redrawn;
 *   - otherwise A CLICK: the ships on the two squares (at most 25) are laid out in 13-per-row tiers
 *     (`get_transit_loc`) and the one under the pointer is found. None clears the drag.
 *     In Colonopedia mode the ship's article opens. A land unit, or a damaged ship, sets state 15.
 *     Otherwise, with the button still down, the ship becomes the dragged unit (state 9): the
 *     drag sprite follows the pointer, and a release over another region is handed to the ship
 *     list and then back here.
 *
 * It compiled RELOCATABLE on the first build.
 *
 * RENAMED 2026-09-27 from europe_voyage_click: CONST PROPOSAL. Mac EuropeWin::EuropeMouseTransit(short, short), CODE 33 0x18dc, 1214 bytes. Callees: EuropeMouseArea x2, attempt_to_set_sail, relocate_unit x2, relocate_stacked_units x2, DrawEurope, top_unit_in_square x2, next_unit x2, get_transit_loc, InBox, LButtonDownNow x2, MSWaitTicks, ShowModal, UnitIcon, DragIcon, EuropeMouseShipsProper, OnOKClick. 3501's callees are that list item for item. Callers: Mac (bsr) EuropeMouseParse, Market, ShipsCargo, ShipsProper, itself; Windows the same five. Arity: this+2 against (w, x, y). Layout: last before EuropeMouseUnits on both (Windows has two 14-byte empty stubs between). Size 1214 against 1298. Constants: overlap 0.84. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "units.h"
#include "w16types.h"
#include "records.h"
#include "classes.h"
#include <stddef.h>
class DlgControl;
template <class T> class LinkedList : public VList { public: LinkedList(); };
class PediaWin : public ColChild { public: short category; SImageButton button; SScrollBar bar; GRPort port; LinkedList<PediaEntry> list; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; PediaWin(); ~PediaWin(); };
typedef char pedia_list_at_0x83e[(offsetof(PediaWin, list) == 0x83e + 58 * (sizeof(void *) - 4)) ? 1 : -1];   /* PORT: +4 a pointer before it on 64-bit (W9) */
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, EuropeWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
#include "records.h"
#define IS_SHIP(t) ((t) >= UNIT_SHIP_FIRST && (t) <= UNIT_SHIP_LAST)

extern char g_pedia_win;               /* SEG20:0x387c */
extern int EuropeMouseArea(int x, int y);
extern void attempt_to_set_sail(int ship);
extern void relocate_unit(int u, int x, int y);
extern void relocate_stacked_units(int u, int x, int y);
extern void EuropeWin_DrawEurope(EuropeWin *w);
extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);
extern void get_transit_loc(int idx, int base_x, int tier3_count, int extra,
                                         int *tier, int *x, int *sprite, int *w, int *size);
extern int InBox(int x, int y, int rx, int ry, int rw, int rh);
extern "C" int Screen_LButtonDownNow(void *w);   /* PORT: C linkage, as defined (W2) */
extern "C" void PediaWin_ShowModal(void *pedia, int category, int type, void *parent);   /* PORT: C linkage, as defined (W2) */
extern "C" int UnitIcon(int u);   /* PORT: C linkage, as defined (W2) */
extern void EuropeWin_OnOKClick(int n);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void ColChild_DragIcon(EuropeWin *w, Sprite *sprite, int x, int y, int *rx, int *ry);
extern "C" void EuropeWin_EuropeMouseShipsProper(EuropeWin *w, int x, int y);

void EuropeWin_EuropeMouseTransit(EuropeWin *w, int x, int y)
{
    int offs[2];
    int base;
    int count;
    int hit;
    int idx;
    int space;
    int next;
    int tier;
    int rx;
    int ry;
    int rw;
    int rh;
    int region;
    int ox;
    int oy;
    char refresh;
    register int u;
    register int i;

    count = 0;
    refresh = 0;
    region = EuropeMouseArea(x, y);
    if (region != 2 && region != 3)
        return;
    if (g_drag_mode == 10)
        return;
    if (g_drag_mode == 8) {
        g_modal_active = 1;
        attempt_to_set_sail(g_europe_selected_ship);
        if (g_modal_active == 0)
            refresh = 1;
    } else if (g_drag_mode == 9) {
        u = g_drag_unit;
        g_units[u].orders = ORDERS_NONE;
        if (g_drag_from != region) {
            space = g_unit_types[g_units[u].type].holds - g_units[u].cargo_count;
            while (u >= 0) {
                next = g_units[u].link2;
                if (g_drag_unit == u)
                    relocate_unit(u, -2, -2);
                else {
                    if (IS_SHIP(g_units[u].type))
                        break;
                    if (g_unit_types[g_units[u].type].size <= (unsigned char)space) {
                        relocate_unit(u, -2, -2);
                        space -= g_unit_types[g_units[u].type].size;
                    }
                }
                u = next;
            }
            if (region == 2)
                relocate_stacked_units(g_drag_unit, g_cur_nation - 0x18, g_cur_nation - 0x18);
            else
                relocate_stacked_units(g_drag_unit, g_cur_nation - 0xc, g_cur_nation - 0xc);
        }
        EuropeWin_DrawEurope(w);
    } else {
        if (region == 2) {
            offs[0] = -0x1c;
            offs[1] = -0x18;
            base = 0x90;
        } else if (region == 3) {
            offs[0] = -0x10;
            offs[1] = -0xc;
            base = 3;
        } else
            goto done;
        for (i = 0; i < 2; i++)
            for (u = top_unit_in_square(offs[i] + g_cur_nation, offs[i] + g_cur_nation); u >= 0; u = next_unit(u))
                count++;
        count = min_int_1048_db44(count, 25);
        if (count == 0) {
            g_drag_mode = 0;
            goto done;
        }
        hit = -1;
        idx = 0;
        for (i = 0; i < 2; i++)
            for (u = top_unit_in_square(offs[i] + g_cur_nation, offs[i] + g_cur_nation); u >= 0 && hit < 0;
                 u = next_unit(u)) {
                get_transit_loc(idx, base, 13, 1, &tier, &rx, &ry, &rw, &rh);
                if (InBox(x, y, rx, ry, rw, rh))
                    hit = u;
                idx++;
            }
        u = hit;
        if (Screen_LButtonDownNow(&(DialogWin &)*w))
            MSWaitTicks_1048_dbd6(15);
        if (g_pedia_mode != 0) {
            if (u >= 0)
                PediaWin_ShowModal((PediaWin *)&g_pedia_win, 1, g_units[u].type, &(DialogWin &)*w);
            goto done;
        }
        if (!IS_SHIP(g_units[u].type) || ((g_units[u].flags2 & 0x80) && g_units[u].type != UNIT_ARTILLERY)) {
            g_drag_mode = 15;
            goto done;
        }
        if (Screen_LButtonDownNow(&(DialogWin &)*w)) {
            g_drag_unit = u;
            g_drag_from = g_drag_mode;
            g_drag_mode = 9;
            ColChild_DragIcon(w, &g_icon_sprites[UnitIcon(u)], x, y, &ox, &oy);
            if (EuropeMouseArea(ox, oy) != region) {
                EuropeWin_EuropeMouseShipsProper(w, ox, oy);
                EuropeWin_EuropeMouseTransit(w, ox, oy);
                g_drag_mode = 0;
            }
        }
    }
done:
    if (refresh)
        EuropeWin_OnOKClick(0);
}
