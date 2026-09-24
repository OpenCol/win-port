/* unit_transfer_cargo_1010_b882 -- shipped at 1010:b882.
 *
 * Ported by tools/port.py from matched/game/unit_transfer_cargo_1010_b882.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern int far g_goods_name_word[];                  SEG20:0xd86e
 *   extern char far g_text_ctx;                          DGROUP:0x009e
 *   extern int far g_cargo_amount;                       DGROUP:0x4bd2
 *   extern int far g_text_args;                          SEG31:0x02d4
 *
 * The file's own Unit, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 min_int_1010_e5d0(col_i16 a, col_i16 b);
void colony_add_goods_text(void *w, col_i16 n);
void colony_show_message(void *w, col_i16 a, col_i32 b);
void colony_window_refresh(void);
void set_text_arg_1010_ebb0(col_i16 slot, col_i16 v);

#include "units.h"
/* 0x1c bytes at SEG20:0xa208 */
/* 14 bytes at SEG20:0x7ac8 */






extern col_i16 unit_cargo_type(col_i16 u, col_i16 slot);
extern col_i16 unit_cargo_room(col_i16 u, col_i16 goods, col_i16 *room);
extern void colony_cancel_then_message(col_i16 n, col_i32 a);
extern col_i16 unit_cargo_amount(col_i16 u, col_i16 slot);
extern void set_name_slot_from_text(col_i16 slot, col_i16 word);
extern col_i16 prompt_text_number(char *ctx, char *name, col_i16 n);
extern col_i16 minimax(col_i16 v, col_i16 lo, col_i16 hi);
extern col_i16 unit_drop_cargo_slot(col_i16 u, col_i16 slot);
extern void unit_add_cargo(col_i16 unit, col_i16 goods, col_i16 amount);
extern void ColChild_CancelMessage(void *w, col_i16 a);
extern void ColChild_AddNumberToMessage(void *w, col_i16 v);
extern void ColChild_AddToMessage(void *w, col_i16 word);

/* same object: reached by the 4-byte `push cs; call ` */






col_i16 unit_transfer_cargo_1010_b882(void *w, register col_i16 from, col_i16 to, col_i16 slot, col_i16 ask)
{
    col_i16 r;
    col_i16 n;
    col_i16 cap;
    register col_i16 goods;

    r = 1;
    goods = unit_cargo_type(from, slot);
    if (unit_cargo_room(to, goods, &n) == 0) {
        colony_cancel_then_message(4, 0x78L);
        goto out;
    }
    n = min_int_1010_e5d0(n, 0x64);
    n = min_int_1010_e5d0(n, unit_cargo_amount(from, slot));
    if (ask != 0) {
        goods = unit_cargo_type(from, slot);
        set_name_slot_from_text(0, g_goods_name_word[goods]);
        set_name_slot_from_text(1, g_unit_types[g_units[from].type].name);
        set_name_slot_from_text(2, g_unit_types[g_units[to].type].name);
        set_text_arg_1010_ebb0(0, n);
        if (prompt_text_number(&(*g_text_ctx), "HOWMUCH3", n) != 0)
            goto out;
        cap = minimax((*g_text_args), 0, n);
        n = min_int_1010_e5d0(cap, n);
        if (n <= 0)
            goto out;
    }
    goods = unit_drop_cargo_slot(from, slot);
    if (goods < 0) {
        colony_cancel_then_message(0xb, 0x78L);
        goto out;
    }
    if (g_cargo_amount > n) {
        unit_add_cargo(from, goods, g_cargo_amount - n);
        g_cargo_amount = n;
    }
    unit_add_cargo(to, goods, g_cargo_amount);
    if (g_units[to].orders != ORDERS_TRADE_ROUTE)
        g_units[to].orders = ORDERS_NONE;
    ColChild_CancelMessage(w, 1);
    ColChild_AddNumberToMessage(w, g_cargo_amount);
    ColChild_AddToMessage(w, g_goods_name_word[goods]);
    colony_add_goods_text(w, 2);
    ColChild_AddToMessage(w, g_unit_types[g_units[to].type].name);
    colony_show_message(w, 1, 0x78L);
    colony_window_refresh();
    r = 0;
out:
    return r;
}
