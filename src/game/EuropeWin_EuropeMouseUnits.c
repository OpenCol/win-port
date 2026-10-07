/* EuropeWin_EuropeMouseUnits: candidate for 1048:3a2f (508 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_EuropeMouseUnits
 * name-source: sibling-build
 * size-source: 508, not the census 499. The trailing 9 bytes are the dead closing-brace epilogue
 * after `done: return;`.
 *
 * Region 4 of the Europe screen: the docks, where the units waiting in Europe stand. Like its
 * neighbour `EuropeWin_EuropeMouseShipsCargo` it is both a click handler and a drop target, and
 * returns unless the point is in its region.
 *
 * A GOODS DROP from a ship's hold (drag source 0) sells them, through `EuropeWin_AttemptCargoToEurope`, the same
 * sale the market and the ship menu use. Boycotted goods -- their bit set in the current nation's
 * mask -- are sold only if `free_boycott` comes back non-zero, i.e. the arrears
 * were paid. A drop from the market (source 1) does nothing.
 *
 * A CLICK finds the unit under the point by laying each of the `DGROUP:0x4cec` dock units out
 * with `get_unit_loc` and testing the point, x shifted 16 to the right, against the cell;
 * the walk stops at the first hit. A still-held button waits 15 ticks first. The unit is then
 * selected (0x1fc0 = 2, its index at 0x4cee) and the screen redrawn; a button STILL held after
 * that calls `europe_select_unit`, which has no body. Finally, a click inside the dock panel's
 * rectangle opens the unit's options menu (`txt_ARMOPTIONS_1048_587e`), or in Colonopedia mode
 * the article for its type, and the screen is redrawn again.
 *
 * UNITS PAST THE EIGHTH CANNOT BE CLICKED. `get_unit_loc` puts them in band 2 and leaves the
 * y out-parameter untouched there, so each is tested at x 0x1d2 and the y left over from the last
 * band-1 cell -- the position of unit 3, which the walk has already tested and would have
 * stopped on. They are not drawn either, so nothing looks wrong. The band itself is fetched into
 * `band` and never read.
 *
 * ONE REGISTER, THREE MEANINGS. DI holds the ship whose goods are sold, the index the walk finds,
 * and then the dock unit that index names. Written as one `register` local reused for all three,
 * the bytes match; SI is the walk's counter.
 *
 * RENAMED 2026-09-26 from europe_dock_click: the Mac build's EuropeWin::EuropeMouseUnits(short, short): this plus x and y, a click on the docks where the units in Europe stand -- ColonyWin_ColonyMouseUnits' counterpart. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
typedef struct { char gamewin[0xc4]; char title[0x80]; int tile_w, tile_h; char save_flag; char own_msg[0xa0]; char queue[0x10][0x50]; char pad_6e9; char btn[0x74]; unsigned char active; unsigned char colchild_kind; int dropped; void *repaint; void *resize; RECT rect; int which; char noclose; char pad_775; int msg_timer; int queued; short category; char button[0x74]; char bar[0x28]; char port[0x26]; char list[0xc]; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; } PediaWin;   /* PediaWin : ColChild, flattened for C (ColChild's kind, which PediaWin's hides, as colchild_kind): 0x85a */   /* PediaWin : ColChild, flattened for C (ColChild's kind, which PediaWin's hides, as colchild_kind): 0x85a */
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, EuropeWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









#include "records.h"

extern char g_pedia_win;               /* SEG20:0x387c, declared */
extern int EuropeMouseArea(int x, int y);
extern int boycott(int bit);
extern int free_boycott(int good);
extern int nth_europe_vessel(int n);

extern int shift_key(void);
extern void get_unit_loc(int index, int x, int *band, int *pos,
                                 int *width, int *p6, int *p7);
extern int InBox(int x, int y, int rx, int ry, int rw, int rh);
extern "C" int Screen_LButtonDownNow(void *w);   /* PORT: C linkage, as defined (W2) */
extern void europe_change_mode(void);
extern void europe_select_unit(void);
extern void europe_click_unit(void);
extern "C" void PediaWin_ShowModal(void *pedia, int category, int type, void *parent);   /* PORT: C linkage, as defined (W2) */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void EuropeWin_AttemptCargoToEurope(EuropeWin *w, int u, int goods, int ask);

void EuropeWin_EuropeMouseUnits(EuropeWin *w, int x, int y)
{
    int band;
    int px;
    int py;
    int cw;
    int ch;
    register int i;
    register int u;

    if (EuropeMouseArea(x, y) != 4)
        return;
    if (g_drag_mode == 10) {
        if (g_goods_drag_from_market == 0) {
            if (boycott(g_selected_goods) != 0 && free_boycott(g_selected_goods) == 0)
                goto done;
            u = nth_europe_vessel(g_europe_selected_ship);
            EuropeWin_AttemptCargoToEurope(w, u, g_selected_goods, shift_key());
        }
    } else {
        u = -1;
        for (i = 0; u < 0 && g_land_unit_count > i; i++) {
            get_unit_loc(i, 0x1d2, &band, &px, &py, &cw, &ch);
            if (InBox(x + 0x10, y, px, py, cw, ch) != 0)
                u = i;
        }
        if (u < 0)
            goto done;
        if (Screen_LButtonDownNow(&(DialogWin &)*w) != 0)
            MSWaitTicks_1048_dbd6(0xf);
        g_selection_kind = 2;
        g_land_unit_index = u;
        europe_change_mode();
        if (Screen_LButtonDownNow(&(DialogWin &)*w) != 0)
            europe_select_unit();
        if (InBox(x, y, 0x1bf, 0x102, 0xc1, 0x93) != 0) {
            if (g_pedia_mode == 0) {
                europe_click_unit();
            } else if (g_land_unit_count != 0) {
                u = nth_land_unit(g_land_unit_index);
                PediaWin_ShowModal((PediaWin *)&g_pedia_win, 1, g_units[u].type, &(DialogWin &)*w);
            }
        }
        europe_change_mode();
    }
done:
    return;
}
