/* colony_load_prompt -- shipped at 1010:b19d.
 *
 * Ported by tools/port.py from matched/game/colony_load_prompt.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern Colony far g_colonies[];                      SEG20:0x7c28
 *   extern int far g_goods_name_word[];                  SEG20:0xd86e
 *   extern int far g_b2;                                 DGROUP:0x00b2
 *   extern int far g_carrier_count;                      DGROUP:0x0c14
 *   extern int far g_cargo_amount;                       DGROUP:0x4bd2
 *   extern int far g_text_args;                          SEG31:0x02d4
 *   extern char far g_text_ctx[];                        DGROUP:0x009e
 *   extern char g_str_bang1[];                           DGROUP:0x0a95
 *   extern char g_str_bang2[];                           DGROUP:0x0a97
 *   extern char g_str_howmuch1[];                        DGROUP:0x0a99
 *
 * The file's own Unit, UnitType, Colony, ColChild dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 min_int_1010_e5d0(col_i16 a, col_i16 b);
void colony_add_goods_text(ColChild *w, col_i16 n);
void colony_show_message(ColChild *w, col_i16 kind, col_i32 ms);
void colony_window_refresh(void);
void set_text_arg_1010_ebb0(col_i16 slot, col_i16 v);

/* 14 bytes */
/* 0xca */
extern void set_current_colony(col_i16 c);
extern void ColChild_CancelMessage(ColChild *w, col_i16 a);
extern void ColChild_AddToMessage(ColChild *w, col_i16 idx);
extern void ColChild_BackspaceMessage(ColChild *w);
extern void ColChild_AddNumberToMessage(ColChild *w, col_i16 v);
extern void message_append(ColChild *w, char *s);
extern col_i16 unit_cargo_room(col_i16 u, col_i16 g, col_i16 *room);
extern void set_name_slot_from_text(col_i16 slot, col_i16 word);
extern col_i16 prompt_text_number(void *a, void *b, col_i16 n);
extern col_i16 minimax(col_i16 a, col_i16 lo, col_i16 hi);
extern void colony_load_goods(col_i16 u, col_i16 g, col_i16 n);

/* same object: 4-byte `push cs; call ` */






col_i16 colony_load_prompt(ColChild *w, register col_i16 u, register col_i16 g,
                                     col_i16 go, col_i16 ask)
{
    col_i16 r;
    col_i16 amount;
    col_i16 cap;

    r = 1;
    set_current_colony(w->colony);
    if (((col_i16 *)((unsigned char *)&(g_colonies[w->colony]) + 0x9a))[g] < 1) {
        ColChild_CancelMessage(w, 1);
        colony_add_goods_text(w, 6);
        ColChild_AddToMessage(w, g_goods_name_word[g]);
        colony_add_goods_text(w, 7);
        colony_show_message(w, 3, 0x78L);
        goto out;
    }
    if (g_b2 != 0 && g_carrier_count == 0) {
        ColChild_CancelMessage(w, 1);
        colony_add_goods_text(w, 5);
        ColChild_AddToMessage(w, g_goods_name_word[g]);
        ColChild_BackspaceMessage(w);
        message_append(w, g_str_bang1);
        colony_show_message(w, 3, 0x78L);
        goto out;
    }
    if (go == 0) {
        r = 0;
        goto out;
    }
    if (unit_cargo_room(u, g, &amount) == 0) {
        ColChild_CancelMessage(w, 1);
        colony_add_goods_text(w, 4);
        ColChild_AddToMessage(w, g_goods_name_word[g]);
        ColChild_BackspaceMessage(w);
        message_append(w, g_str_bang2);
        colony_show_message(w, 3, 0x78L);
        goto out;
    }
    amount = min_int_1010_e5d0(amount, 0x64);
    amount = min_int_1010_e5d0(amount, ((col_i16 *)((unsigned char *)&(g_colonies[w->colony]) + 0x9a))[g]);
    if (ask != 0) {
        set_name_slot_from_text(0, g_goods_name_word[g]);
        set_name_slot_from_text(1, g_unit_types[g_units[u].type].name);
        set_text_arg_1010_ebb0(0, amount);
        if (prompt_text_number(g_text_ctx, g_str_howmuch1, amount) != 0)
            goto out;
        cap = minimax((*g_text_args), 0, amount);
        amount = min_int_1010_e5d0(cap, amount);
        if (amount <= 0)
            goto out;
    }
    colony_load_goods(u, g, amount);
    ColChild_CancelMessage(w, 1);
    ColChild_AddNumberToMessage(w, g_cargo_amount);
    ColChild_AddToMessage(w, g_goods_name_word[g]);
    colony_add_goods_text(w, 2);
    ColChild_AddToMessage(w, g_unit_types[g_units[u].type].name);
    colony_show_message(w, 1, 0x78L);
    if (g_b2 != 0)
        colony_window_refresh();
    r = 0;
out:
    return r;
}
