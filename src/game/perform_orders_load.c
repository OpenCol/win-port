/* perform_orders_load: candidate for 1028:ba03 (703 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 703, not the census 694. The trailing 9 bytes are the dead second epilogue.
 *
 * LOADS THE MOST VALUABLE GOODS IN THE COLONY ONTO A UNIT, and says so through the viceroy.
 * Returns 0 when something was loaded and 1 when nothing was: a full hold answers with message
 * 0x15, an empty colony with 0x14.
 *
 * All sixteen goods are scored as `min(100, stock) * value`, the value being the nation's own
 * byte from the 16-per-nation table at SEG30:0x10 shifted up four -- the same table
 * `capture_cargo` scores a beaten unit's hold with. Goods 0xe and 0xf are worth 1 apiece
 * whatever the table says, and goods 8 is worth nothing at all below 102 in store. The pairs
 * are sorted and walked from the most valuable down; the current nation's own flag can veto a
 * goods, and the first with anything in store wins.
 *
 * Up to 100 of it moves to the unit, and the message names the amount, the goods and the unit:
 * `<qty> <goods>` then message 0x19 and the unit's own name word.
 *
 * `idx[i] = (unsigned char)i` -- the shipped loads the index as a byte and promotes it with
 * `MOV AH,0` before the word store, the same cast `capture_cargo` needed for the same parallel
 * arrays. `min_int_1028_e376` comes through the 4-byte `PUSH CS` form and is a stub here;
 * `boycott` is a FAR call and so extern (rule 47).
 *
 * RENAMED 2026-09-26 from load_best_goods: the Mac build's perform_orders_load(short), bound by call-graph agreement (tools/mac_callgraph.py): 10 of its Mac call edges already bound land on this function (caller KeyClick; callee AddNumberToMessage; callee AddToMessage; callee CancelMessage; callee boycott), the runner-up gets 4, and the arity agrees (1).
 */

#include "colonization.h"

#include "misc.h"
#include "w16types.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

extern GameState g_game;   /* SEG20:0x7782 */
#include "classes.h"

extern int boycott(int bit);
extern void sort_16x16(int n, int *payload, int *key);
extern void set_europe(int n);
extern int stow_cargo(int unit, int goods, int amount);
extern void ColChild_CancelMessage(ColChild *child, int a);

extern void ColChild_AddToMessage(ColChild *child, int word);

/* same object: 4-byte `push cs; call ` */

int perform_orders_load(int u)
{
    int r;
    int space;
    int i;
    int qty;
    int best;
    int idx[16];
    int key[16];
    register int goods_type;
    register int w;

    r = 1;
    best = -1;
    space = g_unit_types[g_units[u].type].holds - g_units[u].cargo_count;
    if (space < 1) {
        viceroy_do_message(0x15, 3);
        goto out;
    }
    for (i = 0; i < 0x10; i++) {
        idx[i] = (unsigned char)i;
        w = g_goods_value[NATION(g_units[u])][i] << 4;
        if (i == 0xf)
            w = 1;
        if (i == 0xe)
            w = 1;
        if (i == 8 && g_cur_colony_rec->stock[i] < 0x66)
            w = 0;
        key[i] = min_int_1028_e376(0x64, g_cur_colony_rec->stock[i]) * w;
    }
    sort_16x16(0x10, idx, key);
    if (g_game.turn_nation == g_game.player_nation)
        set_europe(g_game.player_nation);
    for (i = 0xf; best < 0 && i >= 0; i--) {
        goods_type = idx[i];
        if (g_game.turn_nation == g_game.player_nation && boycott(goods_type) != 0)
            continue;
        if (g_cur_colony_rec->stock[goods_type] > 0)
            best = goods_type;
    }
    if (best < 0) {
        viceroy_do_message(0x14, 3);
        goto out;
    }
    goods_type = best;
    qty = min_int_1028_e376(0x64, g_cur_colony_rec->stock[goods_type]);
    g_cur_colony_rec->stock[goods_type] -= qty;
    stow_cargo(u, goods_type, qty);
    ColChild_CancelMessage((ColChild *)&g_map_window, 1);
    viceroy_message(MISC_LOADING);
    ColChild_AddNumberToMessage((ColChild *)&g_map_window, qty);
    ColChild_AddToMessage((ColChild *)&g_map_window, g_goods_name_word[goods_type]);
    viceroy_message(MISC_ONTO);
    ColChild_AddToMessage((ColChild *)&g_map_window, g_unit_types[g_units[u].type].name);
    viceroy_activate_message(1, 0x78L);
    r = 0;
out:
    return r;
}
