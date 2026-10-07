/* ColonyWin_colony_hammer_buy: candidate for 1010:7388 (757 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_colony_hammer_buy
 * name-source: sibling-build
 *
 * The colony window's "buy" for the item under construction. The game's own prompt is BUYME1:
 * "Cost to complete %STRING0: %NUMBER0. Treasury: %NUMBER1." with "Never mind." and "Complete it.",
 * or BUYME0, the same text with no choice, when the treasury cannot cover it.
 *
 * THE PRICE: 13 per missing hammer, plus (Nation+0x5a + 4) per missing tool, where the nation byte
 * is read as signed and the tools missing are the item's tools less the colony's stock of
 * GOODS_TOOLS -- and the whole price DOUBLED when the colony has put no hammers into the item at all.
 * Nothing is offered for an item `get_production_class` reports as kind 0, or when nothing is missing.
 *
 * Buying ("Complete it.", answer 2) sets the colony's hammers to the item's cost, adds the hammers
 * bought (cost minus what the colony had) to the word at +0x98, charges the treasury, adds the missing tools
 * to the colony's stock, and redraws the colony window.
 *
 * The tools bought are ADDED TO THE COLONY'S STOCK -- the shortfall, not the item's full need --
 * rather than consumed here; this function does not complete the item itself.
 *
 * The window's timer at +0xa10 is reset and the button at +0x972 shown before anything else, and a
 * re-entry byte at DGROUP:0xa05 guards the rest. `ColonyWin_KeyClick` passes a second argument
 * this function declares (`unused1`) and never reads. Its colony record offsets are from SEG20:0x7c28, the base the corpus
 * uses; the record-array heuristic's 0x7c18 is 16 bytes low.
 *
 * RENAMED 2026-09-27 from colony_buy_build: String witness: BUYME0 and BUYME1 are in Mac ColonyWin::colony_hammer_buy(short) and, on Windows, only here. Call graph: the Mac method's sequence is ResetTimer, set_colony, get_production_class, get_production_cost, get_production_name, check_gold x2, popm x2, subtract_gold, colony_change_mode. 7388's callees are the same eleven: ResetTimer, set_colony, get_production_class/cost/name, check_gold x2, popm x2, subtract_gold and colony_change_mode, plus max_int, STRCPY and control_show. Callers: the Mac method is called by ColonyWin::KeyClick, and 7388 is called by ColonyWin_KeyClick (1010:4ccd) and the 7369 thunk. Arity: implicit this plus short, against colony_buy_build(ColonyWin *w, int). Size is 722 Mac against 757 Windows. Apply this only after 1010:7369 has vacated the name.
 */

#include "colonization.h"

#include "goods.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"

extern Nation g_nations[];               /* SEG20:0xc2f4 */

extern void set_colony(int colony);
extern int get_production_class(int item, int *idx);
extern int get_production_cost(int item, int *tools);
extern char *get_production_name(int item);

extern long check_gold(int nation);
extern int popm(char *msg, int n);
extern void subtract_gold(int nation, long amount);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void MSShowControl_1010_e574(void *control);

extern "C" void colony_change_mode(void);

void ColonyWin_colony_hammer_buy(ColonyWin *w, int unused1)
{
    int idx;
    int kind;
    int cost;
    int hshort;
    int r;
    int hammers_bought;
    int tools;
    register int price;
    register int tshort;

    if (w->timer != -1) {
        ResetTimer(w->timer);
        w->timer = -1;
    }
    MSShowControl_1010_e574(&w->b[1]);
    if (g_buy_busy != 0)
        return;
    g_buy_busy = 1;
    set_colony(w->colony);
    kind = get_production_class(g_colonies[w->colony].item, &idx);
    cost = get_production_cost(g_colonies[w->colony].item, &tools);
    tshort = tools - g_colonies[w->colony].stock[GOODS_TOOLS];
    tshort = max_int_1010_e632(0, tshort);
    if (kind == 0)
        goto done;
    hshort = cost - g_colonies[w->colony].hammers;
    hshort = max_int_1010_e632(0, hshort);
    if (hshort <= 0 && tshort == 0)
        goto done;
    price = hshort * 13;
    if (tshort != 0)
        price += (g_nations[g_colonies[w->colony].nation].price[GOODS_TOOLS] + 4) * tshort;
    if (g_colonies[w->colony].hammers == 0)
        price = price * 2;
    STRCPY(g_name_slots[0], get_production_name(g_colonies[w->colony].item));
    g_text_args[0] = (long)price;
    g_text_args[1] = check_gold(g_colonies[w->colony].nation);
    if (check_gold(g_colonies[w->colony].nation) >= (long)price)
        r = popm("BUYME1", 5);
    else {
        popm("BUYME0", 5);
        r = 0;
    }
    if (r != 2)
        goto done;
    hammers_bought = cost - g_colonies[w->colony].hammers;
    if (hammers_bought > 0)
        g_colonies[w->colony].bonus_hammers += hammers_bought;
    g_colonies[w->colony].hammers = cost;
    subtract_gold(g_colonies[w->colony].nation, (long)price);
    if (tools != 0)
        g_colonies[w->colony].stock[GOODS_TOOLS] += tshort;
    colony_change_mode();
done:
    g_buy_busy = 0;
}
