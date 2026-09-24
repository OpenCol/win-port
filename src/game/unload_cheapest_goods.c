/* unload_cheapest_goods -- shipped at 1028:b728.
 *
 * Ported by tools/port.py from matched/game/unload_cheapest_goods.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *   extern unsigned char far g_goods_value[];            SEG30:0x0010
 *   extern int far g_goods_name_word[];                  SEG20:0xd86e
 *   extern int far g_cargo_amount;                       DGROUP:0x4bd2
 *   extern char far g_map_window[];                      SEG20:0x0658
 *   extern char far g_europe_win;                        SEG20:0x2fd2
 *
 * The file's own Unit, Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void set_name_slot_1028_e412(col_i16 i, char *s);
void set_text_arg_i_1028_e3ee(col_i16 i, col_i16 v);

#include "goods.h"

#define NATION(u) ((unsigned char)((u).flags & 0xf))
extern void viceroy_do_message(col_i16 a, col_i16 b);
extern col_i16 unit_cargo_type(col_i16 u, col_i16 slot);
extern col_i16 unit_cargo_amount(col_i16 u, col_i16 slot);
extern void sort_int_pairs_by_key(col_i16 n, col_i16 *ids, col_i16 *keys);
extern col_i16 colony_warehouse_capacity(void);
extern void set_name_slot_from_text(col_i16 slot, col_i16 word);
extern col_i16 text_message_show(char *msg, col_i16 n);
extern void europe_sell_cargo(char *w, col_i16 u, col_i16 goods, col_i16 ask);
extern col_i16 unit_drop_cargo_slot(col_i16 u, col_i16 slot);
extern void ColChild_CancelMessage(void *w, col_i16 a);
extern void viceroy_message(col_i16 n);
extern void ColChild_AddNumberToMessage(void *w, col_i16 v);
extern void ColChild_AddToMessage(void *w, col_i16 word);
extern void message_append(void *w, char *s);
extern void viceroy_activate_message(col_i16 a, col_i32 b);

/* same object: 4-byte `push cs; call ` */



col_i16 unload_cheapest_goods(col_i16 u, col_i16 in_europe)
{
    col_i16 r;
    col_i16 i;
    col_i16 count;
    col_i16 amount;
    col_i16 w;
    col_i16 worst;
    col_i16 cap;
    col_i16 idx[6];
    col_i16 val[6];
    register col_i16 goods;

    r = 1;
    worst = -1;
    count = g_units[u].cargo_count;
    if (count < 1) {
        viceroy_do_message(0x16, 3);
        goto out;
    }
    for (i = 0; i < count; i++) {
        idx[i] = (unsigned char)i;
        goods = unit_cargo_type(u, i);
        amount = unit_cargo_amount(u, i);
        w = ((unsigned char *)g_goods_value)[(NATION(g_units[u]) << 4) + goods] << 4;
        if (goods == GOODS_MUSKETS)
            w = 0;
        if (goods == GOODS_TOOLS)
            w = 0;
        if (goods == GOODS_HORSES)
            w = 1;
        val[i] = w * amount;
    }
    sort_int_pairs_by_key(count, idx, val);
    worst = idx[0];
    if (worst < 0) {
        viceroy_do_message(0x14, 3);
        goto out;
    }
    goods = unit_cargo_type(u, worst);
    amount = unit_cargo_amount(u, worst);
    if (in_europe == 0) {
        cap = colony_warehouse_capacity();
        if (((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[goods] + amount > cap && goods != GOODS_FOOD) {
            set_name_slot_1028_e412(0, ((char *)((unsigned char *)(g_cur_colony_rec) + 0x2)));
            set_name_slot_from_text(1, g_goods_name_word[goods]);
            set_text_arg_i_1028_e3ee(0, ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[goods]);
            set_text_arg_i_1028_e3ee(1, cap);
            set_text_arg_i_1028_e3ee(2, amount);
            if (text_message_show("WAREHOUSEFULL", 5) != 2)
                goto out;
        }
    }
    if (in_europe != 0) {
        goods = unit_cargo_type(u, worst);
        europe_sell_cargo(&g_europe_win, u, goods, 0);
    } else {
        goods = unit_drop_cargo_slot(u, worst);
        ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[goods] += g_cargo_amount;
        ColChild_CancelMessage((&g_map_window), 1);
        viceroy_message(0x18);
        ColChild_AddNumberToMessage((&g_map_window), g_cargo_amount);
        ColChild_AddToMessage((&g_map_window), g_goods_name_word[goods]);
        viceroy_message(0x1a);
        message_append((&g_map_window), ((char *)((unsigned char *)(g_cur_colony_rec) + 0x2)));
        viceroy_activate_message(1, 0x78L);
    }
    r = 0;
out:
    return r;
}
