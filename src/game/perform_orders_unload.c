/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 113 of the imported file. */
/* perform_orders_unload: candidate for 1028:b728 (731 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 731, not the census 722. The trailing 9 bytes are the dead second epilogue.
 *
 * UNLOADS ONE CARGO SLOT, THE LEAST VALUABLE, from a unit, and says so through the viceroy. It is
 * the mirror of `perform_orders_load`, its neighbour 0x2db bytes on: that one scores the
 * colony's goods and loads the best, this one scores the unit's hold and unloads the worst.
 * Returns 0 when something moved and 1 when nothing did: an empty hold answers with message 0x16.
 *
 * Every slot is scored `value * amount`, the value being the nation's byte from the
 * 16-per-nation table at SEG30:0x10 shifted up four -- the table `capture_cargo` and
 * `perform_orders_load` read. The overrides run the other way from the loader's: tools and muskets are
 * worth NOTHING, so they come off first, and horses are worth 1 apiece. The pairs are sorted and
 * the first -- the cheapest -- is taken.
 *
 * In a colony (`in_europe == 0`) a load that would overflow the warehouse asks first, with
 * WAREHOUSEFULL naming the colony, the goods, the stock, the capacity and the amount; any answer
 * but 2 keeps the goods aboard. Food is never checked. The goods then go into the colony's stock
 * by the amount `unload_cargo` leaves at DGROUP:0x4bd2, and the message is
 * `<qty> <goods>`, message 0x1a, and the colony's name. In Europe the slot is SOLD instead,
 * through `EuropeWin_AttemptCargoToEurope` on the Europe window -- the call the dock's goods drop makes
 * (`EuropeWin_EuropeMouseUnits`).
 *
 * `idx[i] = (unsigned char)i` is the byte-promoted index both neighbours needed. The two
 * text-argument setters come through the 4-byte `PUSH CS` form and are stubs here.
 *
 * RENAMED 2026-09-26 from unload_cheapest_goods: the Mac build's perform_orders_unload(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 9 of its Mac call edges already bound land on this function (caller KeyClick; callee AddNumberToMessage; callee AddToMessage; callee CancelMessage; callee get_cargo), the runner-up gets 7, and the arity agrees (2).
 */

#include "colonization.h"

#include "goods.h"
#include "misc.h"

#include "w16types.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

#include "classes.h"

typedef struct { char pad[1]; } EuropeWin;

extern int get_cargo(int u, int slot);
extern int get_cargo_amount(int u, int slot);
extern void sort_16x16(int n, int *payload, int *key);
extern int get_warehouse(void);
extern void popup_say(int slot, int word);
extern int popm(char *msg, int n);
extern void EuropeWin_AttemptCargoToEurope(EuropeWin *w, int u, int goods, int ask);
extern int unload_cargo(int u, int slot);
extern void ColChild_CancelMessage(ColChild *child, int a);

extern void ColChild_AddToMessage(ColChild *child, int word);
extern void ColChild_AddStringToMessage(ColChild *message, char *s);

/* same object: 4-byte `push cs; call ` */

int perform_orders_unload(int u, int in_europe)
{
    int r;
    int i;
    int count;
    int amount;
    int w;
    int worst;
    int cap;
    int idx[6];
    int key[6];
    register int goods;

    r = 1;
    worst = -1;
    count = g_units[u].cargo_count;
    if (count < 1) {
        viceroy_do_message(0x16, 3);
        goto out;
    }
    for (i = 0; i < count; i++) {
        idx[i] = (unsigned char)i;
        goods = get_cargo(u, i);
        amount = get_cargo_amount(u, i);
        w = g_goods_value[NATION(g_units[u])][goods] << 4;
        if (goods == GOODS_MUSKETS)
            w = 0;
        if (goods == GOODS_TOOLS)
            w = 0;
        if (goods == GOODS_HORSES)
            w = 1;
        key[i] = w * amount;
    }
    sort_16x16(count, idx, key);
    worst = idx[0];
    if (worst < 0) {
        viceroy_do_message(0x14, 3);
        goto out;
    }
    goods = get_cargo(u, worst);
    amount = get_cargo_amount(u, worst);
    if (in_europe == 0) {
        cap = get_warehouse();
        if (g_cur_colony_rec->stock[goods] + amount > cap && goods != GOODS_FOOD) {
            set_name_slot_1028_e412(0, g_cur_colony_rec->name);
            popup_say(1, g_goods_name_word[goods]);
            set_text_arg_i_1028_e3ee(0, g_cur_colony_rec->stock[goods]);
            set_text_arg_i_1028_e3ee(1, cap);
            set_text_arg_i_1028_e3ee(2, amount);
            if (popm("WAREHOUSEFULL", 5) != 2)
                goto out;
        }
    }
    if (in_europe != 0) {
        goods = get_cargo(u, worst);
        EuropeWin_AttemptCargoToEurope((EuropeWin *)(&g_europe_win), u, goods, 0);
    } else {
        goods = unload_cargo(u, worst);
        g_cur_colony_rec->stock[goods] += g_cargo_amount;
        ColChild_CancelMessage((ColChild *)&g_map_window, 1);
        viceroy_message(MISC_UNLOADING);
        ColChild_AddNumberToMessage((ColChild *)&g_map_window, g_cargo_amount);
        ColChild_AddToMessage((ColChild *)&g_map_window, g_goods_name_word[goods]);
        viceroy_message(MISC_IN);
        ColChild_AddStringToMessage((ColChild *)&g_map_window, g_cur_colony_rec->name);
        viceroy_activate_message(1, 0x78L);
    }
    r = 0;
out:
    return r;
}
