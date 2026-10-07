/* ColonyWin_DrawMakeList: candidate for 1010:78cb (691 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DrawMakeList
 * name-source: sibling-build
 *
 * Builds the colony window's three production rows. Each row is a list pushed one entry at a
 * time -- `ResetMultiIcon` starts it, `AddMultiIcon` adds an entry, `DrawMultiIconRow`
 * draws it at (ox + 0x1b0, oy + y) 0xb0 wide -- and `y` steps 0x20 between them.
 *
 * Row one is the raw materials, goods 0..7 with food (0) and lumber (5) held back; row two the
 * manufactured goods, 8..0xf, each paired through the byte table at SEG21:0xa6 with the raw
 * good it consumes, so the entry carries the larger of what was made and what the input
 * overflowed. Row three is lumber and hammers, and it is the only one with structure: lumber
 * over the stored value splits into the stored part and the excess, lumber under it goes in
 * whole with the overflow as a SECOND entry under kind 0x801c -- the high bit of the kind is
 * what marks an entry as a shortfall, and 0x37 / 0x8037 do the same for hammers.
 *
 * WHICH ENTRY OF EACH TABLE THE THIRD ROW READS IS NOT SOMETHING THE COMPARATOR CHECKS: a
 * constant index folds into the fixup that carries the table's address, so `[5]` and `[16]`
 * were read off the shipped 0x12c, 0x142, 0x16e, 0x196, 0x1be against the 0x122, 0x164, 0x18c
 * and 0x1b4 bases (§119).
 *
 * `(int)g_goods_input[i] >= 0` needs the cast: without it Borland tests the byte in place
 * (`CMP BYTE [SI+0xa6],0`) where the shipped loads, `CBW`s and tests the word.
 *
 * The comparison in row three is UNSIGNED (`JNA`): the lumber production is compared as
 * `(unsigned)`, the width `needs_a_building` gives the table.
 *
 * RENAMED 2026-09-26 from colony_draw_production: the Mac build's ColonyWin::DrawProduction(void): this only, building the three production rows.
 *
 * RENAMED 2026-09-27 from ColonyWin_DrawProduction: DISAGREE, SETTLED AS A SWAP with 7edb. Mac DrawMakeList(void), CODE 30 0x0b04, 612 bytes: set_colony, then three rows, each ResetMultiIcon, AddMultiIcon..., DrawMultiIconRow (3 rows, 8 AddMultiIcon). 78cb calls set_colony, ResetMultiIcon x3, AddMultiIcon x8 and DrawMultiIconRow x3. It is the goods-made list. Mac DrawProduction instead calls get_production_cost/get_production_name/DrawIconRow, which is 7edb's set. Layout: Mac GenerateColonyBaseMap, DrawMakeList, DrawColonyUnits, DrawProduction; Windows 767d, 78cb, DrawColonyUnits, 7edb, so 78cb holds DrawMakeList's slot. Caller: DrawMiscellaneous on both. Arity: this against (ColonyWin *w). Size 612 against 691. Constants: rare -0x7fe4 and -0x7fc9 are shared only with Mac DrawMakeList (overlap 0.93), and 0.11 with the current partner. The earlier binding was a paraphrase ('the three production rows'). Swap through a temporary.
 */

#include "colonization.h"

#include "w16types.h"
#include "goods.h"
#include "classes.h"

extern int g_consumption[];                  /* SEG21:0x164 */
extern int g_deficit[];                /* SEG21:0x18c */
extern int g_shortfall[];              /* SEG21:0x1b4 */

extern int g_stub_a;
extern void set_colony(int colony);
extern void ResetMultiIcon(void);
extern void AddMultiIcon(int kind, int count, int part);
extern void DrawMultiIconRow(GRPort *w, int x, int y, int width, int style);

/* same object: 4-byte `push cs; call ` */

void ColonyWin_DrawMakeList(ColonyWin *w)
{
    int goods;
    int y;
    register int i;
    register int shortfall;

    set_colony(w->colony);
    g_draw_labels = g_show_icon_counts;
    y = 0x114;
    ResetMultiIcon();
    for (i = 0; i <= 7; i++) {
        goods = i;
        if (g_prod[i] != 0 && goods != 5 && goods != 0)
            AddMultiIcon(goods + 0x17, g_prod[i] + g_deficit[i], g_deficit[i]);
    }
    DrawMultiIconRow((GRPort *)w, w->rect.left + 0x1b0, w->rect.top + y, 0xb0, 2);
    y += 0x20;
    ResetMultiIcon();
    for (i = 8; i <= 0xf; i++) {
        shortfall = 0;
        if ((int)g_goods_input[i] >= 0)
            shortfall = g_shortfall[g_goods_input[i]];
        if (g_prod[i] != 0 || shortfall != 0)
            AddMultiIcon(i + 0x17, max_int_1010_e632(g_prod[i], shortfall), shortfall);
    }
    DrawMultiIconRow((GRPort *)w, w->rect.left + 0x1b0, w->rect.top + y, 0xb0, 2);
    y += 0x20;
    ResetMultiIcon();
    if ((unsigned)g_prod[GOODS_LUMBER] > g_consumption[GOODS_LUMBER]) {
        if (g_consumption[GOODS_LUMBER] != 0)
            AddMultiIcon(0x1c, g_consumption[GOODS_LUMBER], 0);
        AddMultiIcon(0x1c, g_prod[GOODS_LUMBER] - g_consumption[GOODS_LUMBER], 0);
    } else {
        if (g_prod[GOODS_LUMBER] != 0)
            AddMultiIcon(0x1c, g_prod[GOODS_LUMBER], 0);
        if (g_deficit[GOODS_LUMBER] != 0)
            AddMultiIcon(0x801c, g_deficit[GOODS_LUMBER], 0);
    }
    if ((unsigned)g_prod[16] - g_shortfall[GOODS_LUMBER] != 0)
        AddMultiIcon(0x37, g_prod[16] - g_shortfall[GOODS_LUMBER], 0);
    if (g_shortfall[GOODS_LUMBER] != 0)
        AddMultiIcon(0x8037, g_shortfall[GOODS_LUMBER], 0);
    DrawMultiIconRow((GRPort *)w, w->rect.left + 0x1b0, w->rect.top + y, 0xb0, 4);
    g_draw_labels = 0;
}
