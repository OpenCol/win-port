/* europe_transfer_cargo -- shipped at 1048:2979.
 *
 * Ported by tools/port.py from matched/game/europe_transfer_cargo.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern int far g_goods_name_word[];                  SEG20:0xd86e
 *   extern char far g_text_ctx;                          DGROUP:0x009e
 *   extern int far g_cargo_amount;                       DGROUP:0x4bd2
 *   extern long far g_text_args[];                       SEG31:0x02d4
 *
 * The file's own Unit, UnitType, EuropeWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 min_int_1048_db44(col_i16 a, col_i16 b);
void ColChild_AddNumberToMessage(EuropeWin *w, col_i16 v);
void ColChild_AddToMessage(EuropeWin *w, col_i16 word);
void ColChild_BackspaceMessage(EuropeWin *w);
void ColChild_CancelMessage(EuropeWin *w, col_i16 a);
void dialog_set_option_value(col_i16 i, col_i16 v);
void message_append(EuropeWin *w, char *s);
void msg_activate_redraw(EuropeWin *w, col_i16 kind, col_i32 arg);
void msg_add_nation_word(EuropeWin *w, col_i16 n);
void show_nation_message(EuropeWin *w, col_i16 n, col_i32 arg);

extern col_i16 unit_cargo_type(col_i16 u, col_i16 slot);
extern col_i16 unit_cargo_room(col_i16 u, col_i16 goods, col_i16 *room);
extern col_i16 unit_cargo_amount(col_i16 u, col_i16 slot);
extern void set_name_slot_from_text(col_i16 slot, col_i16 word);
extern col_i16 prompt_text_number(char *ctx, char *name, col_i16 n);
extern col_i16 minimax(col_i16 v, col_i16 lo, col_i16 hi);
extern col_i16 unit_drop_cargo_slot(col_i16 u, col_i16 slot);
extern void unit_add_cargo(col_i16 unit, col_i16 goods, col_i16 amount);

/* 5-byte NOP form, so extern (rule 47) */
extern void set_current_nation(col_i16 n);
extern void sub_1048_4e81(void);

/* same object: 4-byte `push cs; call ` (rule 47) */











col_i16 europe_transfer_cargo(EuropeWin *w, col_i16 src, col_i16 to, col_i16 slot, col_i16 ask)
{
    register col_i16 from = src;
    register col_i16 g;
    col_i16 r;
    col_i16 goods;
    col_i16 n;
    col_i16 cap;

    r = 1;
    set_current_nation(w->nation);
    goods = unit_cargo_type(from, slot);
    if (unit_cargo_room(to, goods, &n) == 0) {
        goods = unit_cargo_type(to, slot);
        ColChild_CancelMessage(w, 1);
        msg_add_nation_word(w, 4);
        ColChild_AddToMessage(w, g_goods_name_word[goods]);
        ColChild_BackspaceMessage(w);
        message_append(w, "!");
        msg_activate_redraw(w, 3, 0x78L);
        goto out;
    }
    n = min_int_1048_db44(n, 0x64);
    n = min_int_1048_db44(n, unit_cargo_amount(from, slot));
    if (ask != 0) {
        g = unit_cargo_type(from, slot);
        set_name_slot_from_text(0, g_goods_name_word[g]);
        set_name_slot_from_text(1, g_unit_types[g_units[from].type].name);
        set_name_slot_from_text(2, g_unit_types[g_units[to].type].name);
        dialog_set_option_value(0, n);
        if (prompt_text_number(&(*g_text_ctx), "HOWMUCH3", n) != 0)
            goto out;
        cap = minimax((col_i16)g_text_args[0], 0, n);
        n = min_int_1048_db44(cap, n);
        if (n <= 0)
            goto out;
    }
    g = unit_drop_cargo_slot(from, slot);
    if (g < 0) {
        show_nation_message(w, 0xb, 0x78L);
        goto out;
    }
    if (g_cargo_amount > n) {
        unit_add_cargo(from, g, g_cargo_amount - n);
        g_cargo_amount = n;
    }
    unit_add_cargo(to, g, g_cargo_amount);
    ColChild_CancelMessage(w, 1);
    ColChild_AddNumberToMessage(w, g_cargo_amount);
    ColChild_AddToMessage(w, g_goods_name_word[g]);
    msg_add_nation_word(w, 2);
    ColChild_AddToMessage(w, g_unit_types[g_units[to].type].name);
    msg_activate_redraw(w, 1, 0x78L);
    sub_1048_4e81();
    r = 0;
out:
    return r;
}
